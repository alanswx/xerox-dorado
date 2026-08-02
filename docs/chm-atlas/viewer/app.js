(() => {
  const seed = window.ATLAS_SEED || {};
  const seedSets = {
    artifacts: seed.artifacts || [],
    releases: seed.releases || [],
    recipes: seed.recipes || [],
    gaps: seed.gaps || []
  };
  const state = { remote: window.ATLAS_REMOTE || [], rows: [], browser: { server: "", path: "" }, browserIndex: null };
  const $ = id => document.getElementById(id);

  function e(value) {
    return String(value ?? "").replace(/[&<>"']/g, char => ({"&":"&amp;","<":"&lt;",">":"&gt;","\"":"&quot;","'":"&#39;"}[char]));
  }

  function label(record, type) {
    return record.filename || record.name || record.goal || record.claim || record.id || type;
  }

  function path(record) {
    return record.archive_path || record.local_path || record.id || "";
  }

  function status(record) {
    return record.mirror_status || record.status || "unclassified";
  }

  function badge(value) {
    const good = /working|observed|mirrored|complete|high/i.test(value);
    const missing = /missing|unmatched|incomplete|candidate/i.test(value);
    return `<span class="badge ${good ? "good" : missing ? "missing" : ""}">${e(value)}</span>`;
  }

  function evidenceLinks(record) {
    return (record.evidence || []).map(item => {
      if (!item.startsWith("docs/")) return `<li>${e(item)}</li>`;
      return `<li><a href="../../${e(item.slice(5))}">${e(item)}</a></li>`;
    }).join("");
  }

  function metric(title, value, note) {
    return `<div class="metric"><strong>${e(value)}</strong><span>${e(title)}</span><small>${e(note)}</small></div>`;
  }

  function recipesFor(system) {
    return (seed.recipes || []).filter(recipe => recipe.system === system);
  }

  function environmentCard(release) {
    const recipes = recipesFor(release.name).concat(recipesFor(release.family));
    const recipe = recipes[0];
    const artifacts = (release.primary_artifacts || []).map(id => `<li><code>${e(id)}</code></li>`).join("");
    return `<article class="card environment-card">
      <div class="card-kicker">${e(release.family)} · ${e(release.target)}</div>
      <h3>${e(release.name)}</h3>
      <p>${badge(release.status)}</p>
      <p>${e(release.notes || "")}</p>
      ${recipe ? `<div class="chain"><strong>Boot chain</strong><ol>${(recipe.boot_path || []).map(step => `<li>${e(step)}</li>`).join("")}</ol></div>` : ""}
      <details><summary>Primary artifacts (${(release.primary_artifacts || []).length})</summary><ul>${artifacts}</ul></details>
      <div class="card-foot">${e((release.evidence || []).join(" · "))}</div>
    </article>`;
  }

  function renderOverview() {
    const exact = state.remote.filter(item => item.mirror_status === "mirrored-exact-name-and-content").length;
    const same = state.remote.filter(item => item.mirror_status === "mirrored-same-content-different-name").length;
    const unmatched = state.remote.filter(item => item.mirror_status === "not-mirrored-or-unmatched").length;
    $("metrics").innerHTML = [
      metric("CHM occurrences", state.remote.length.toLocaleString(), "one record per archive copy"),
      metric("Exact local matches", exact.toLocaleString(), "name, size, and CRC32"),
      metric("Same-content matches", same.toLocaleString(), "copied or renamed locally"),
      metric("Unmatched occurrences", unmatched.toLocaleString(), "research leads, not proof of absence"),
      metric("Curated environments", (seed.releases || []).length, "release-level interpretations")
    ].join("");
    $("overview-environments").innerHTML = (seed.releases || []).slice(0, 6).map(environmentCard).join("");
  }

  function renderEnvironments() {
    $("environments").innerHTML = (seed.releases || []).map(environmentCard).join("");
  }

  function buildBrowserIndex() {
    const index = new Map();
    const makeNode = (server, path) => {
      const key = `${server}|${path}`;
      if (!index.has(key)) index.set(key, { key, server, path, files: [], children: new Map(), total: 0, unmatched: 0 });
      return index.get(key);
    };
    state.remote.forEach(record => {
      const server = record.server || "unknown";
      let parts = (record.cross_reference_path || "").split("/").filter(Boolean);
      if (parts.length && parts[0].toLowerCase() === server.toLowerCase()) parts = parts.slice(1);
      let parent = makeNode(server, "");
      parent.total++;
      if (record.mirror_status === "not-mirrored-or-unmatched") parent.unmatched++;
      let path = "";
      parts.forEach(part => {
        path = path ? `${path}/${part}` : part;
        const child = makeNode(server, path);
        parent.children.set(part, child);
        child.total++;
        if (record.mirror_status === "not-mirrored-or-unmatched") child.unmatched++;
        parent = child;
      });
      parent.files.push(record);
    });
    return index;
  }

  function browserNode() {
    if (!state.browserIndex) state.browserIndex = buildBrowserIndex();
    return state.browserIndex.get(`${state.browser.server}|${state.browser.path}`);
  }

  function browserDirectoryName(path) {
    if (!path) return "root";
    return path.split("/").pop();
  }

  function browserDetail(record) {
    const entries = Object.entries(record).filter(([, value]) => value !== null && value !== undefined && value !== "");
    return `<h3>${e(record.filename || record.id)}</h3>${entries.map(([key, value]) => `<dl><dt>${e(key)}</dt><dd>${e(typeof value === "object" ? JSON.stringify(value, null, 2) : value)}</dd>`).join("")}`;
  }

  function renderBrowser() {
    if (!state.browserIndex) state.browserIndex = buildBrowserIndex();
    const servers = [...new Set(state.remote.map(record => record.server || "unknown"))].sort();
    const serverSelect = $("browser-server");
    const oldServer = serverSelect.value;
    serverSelect.innerHTML = '<option value="">All servers</option>' + servers.map(server => `<option value="${e(server)}">${e(server)}</option>`).join("");
    if (servers.includes(state.browser.server || oldServer)) serverSelect.value = state.browser.server || oldServer;
    state.browser.server = serverSelect.value;
    if (!state.browser.server) state.browser.path = "";
    const current = browserNode();
    const currentPath = state.browser.path;
    const branchPrefix = (record) => {
      const parts = (record.cross_reference_path || "").split("/").filter(Boolean);
      if (parts.length && parts[0].toLowerCase() === state.browser.server.toLowerCase()) parts.shift();
      return parts.join("/");
    };
    const query = $("browser-search").value.trim().toLowerCase();
    const unmatched = $("browser-unmatched").checked;
    let files = current ? current.files : [];
    let childDirs = current ? [...current.children.values()].sort((a, b) => a.path.localeCompare(b.path)) : [];
    if (query || unmatched) {
      const prefix = currentPath ? `${currentPath}/` : "";
      files = state.remote.filter(record => {
        if (state.browser.server && (record.server || "unknown") !== state.browser.server) return false;
        const branch = branchPrefix(record);
        if (currentPath && branch !== currentPath && !branch.startsWith(prefix)) return false;
        if (unmatched && record.mirror_status !== "not-mirrored-or-unmatched") return false;
        return !query || JSON.stringify(record).toLowerCase().includes(query);
      }).sort((a, b) => a.filename.localeCompare(b.filename));
      childDirs = [];
    } else if (!state.browser.server) {
      childDirs = [];
    }
    const crumbs = [];
    if (state.browser.server) {
      crumbs.push(`<button data-browser-crumb="">${e(state.browser.server)}</button>`);
      let path = "";
      currentPath.split("/").filter(Boolean).forEach(part => {
        path = path ? `${path}/${part}` : part;
        crumbs.push(`<span>›</span><button data-browser-crumb="${e(path)}">${e(part)}</button>`);
      });
    } else crumbs.push(`<span class="crumb-root">All servers</span>`);
    $("browser-breadcrumbs").innerHTML = crumbs.join("");
    $("browser-dirs").innerHTML = !state.browser.server
      ? servers.map(server => `<button class="browser-dir" data-browser-server="${e(server)}"><strong>${e(server)}</strong><small>${state.browserIndex.get(`${server}|`)?.total.toLocaleString() || 0} occurrences</small></button>`).join("")
      : childDirs.map(child => `<button class="browser-dir" data-browser-path="${e(child.path)}"><strong>📁 ${e(browserDirectoryName(child.path))}</strong><small>${child.total.toLocaleString()} files · ${child.unmatched.toLocaleString()} unmatched</small></button>`).join("") || '<p class="empty">No subdirectories.</p>';
    $("browser-summary").textContent = state.browser.server ? `${state.browser.server} / ${currentPath || "root"} · ${files.length.toLocaleString()} files shown${query || unmatched ? " by filter" : ""}` : `${servers.length} CHM servers · choose a server to browse.`;
    $("browser-rows").innerHTML = files.slice(0, 500).map((record, index) => `<tr data-browser-file="${index}"><td><code>${e(record.filename)}</code></td><td>${e((record.kind || []).join(", "))}</td><td>${record.bytes?.toLocaleString?.() || ""}</td><td>${e(record.created || "")}</td><td>${badge(record.mirror_status)}</td></tr>`).join("") || '<tr><td colspan="5">No files in this directory.</td></tr>';
    if (files.length > 500) $("browser-summary").textContent += ` · showing first 500`;
    [...$("browser-dirs").querySelectorAll("[data-browser-server]")].forEach(button => button.addEventListener("click", () => { state.browser.server = button.dataset.browserServer; state.browser.path = ""; renderBrowser(); }));
    [...$("browser-dirs").querySelectorAll("[data-browser-path]")].forEach(button => button.addEventListener("click", () => { state.browser.path = button.dataset.browserPath; renderBrowser(); }));
    [...$("browser-breadcrumbs").querySelectorAll("[data-browser-crumb]")].forEach(button => button.addEventListener("click", () => { state.browser.path = button.dataset.browserCrumb; renderBrowser(); }));
    [...$("browser-rows").querySelectorAll("[data-browser-file]")].forEach(row => row.addEventListener("click", () => {
      const record = files[Number(row.dataset.browserFile)];
      $("browser-rows").querySelectorAll("tr").forEach(item => item.classList.remove("selected"));
      row.classList.add("selected");
      $("browser-detail").innerHTML = browserDetail(record);
    }));
  }

  function renderRecipes() {
    $("recipes").innerHTML = (seed.recipes || []).map(recipe => `<article class="recipe card">
      <div class="card-kicker">${e(recipe.system)} · ${e(recipe.target)}</div>
      <h3>${e(recipe.goal)}</h3>
      <p>${badge(recipe.status)}</p>
      <div class="recipe-columns">
        <div><h4>Inputs</h4><ul>${(recipe.inputs || []).map(input => `<li><span class="role">${e(input.role)}</span><br><code>${e(input.artifact)}</code></li>`).join("")}</ul></div>
        <div><h4>Ordered boot path</h4><ol>${(recipe.boot_path || []).map(step => `<li>${e(step)}</li>`).join("")}</ol></div>
        <div><h4>Media and network</h4><p><strong>Format:</strong> ${e(recipe.media?.format || "unspecified")}</p><p><strong>Network:</strong> ${recipe.network?.required ? `required (${e((recipe.network.services || []).join(", "))})` : "not required"}</p></div>
      </div>
      <details><summary>Known constraints and open questions</summary><ul>${(recipe.known_incompatibilities || []).concat(recipe.open_questions || []).map(item => `<li>${e(item)}</li>`).join("")}</ul></details>
      <div class="card-foot">Gate: <code>${e(recipe.validation?.gate || "not specified")}</code></div>
    </article>`).join("");
  }

  function renderGaps() {
    $("gaps").innerHTML = (seed.gaps || []).map(gap => `<article class="gap card">
      <div class="card-kicker">${e(gap.id)} · ${e(gap.scope)}</div>
      <h3>${e(gap.claim)}</h3>
      <p><strong>Impact:</strong> ${e(gap.impact)}</p>
      <p><strong>Next:</strong> ${e(gap.next_action)}</p>
      <div>${badge(gap.confidence)} <span class="as-of">checked ${e(gap.checked_as_of)}</span></div>
      <details><summary>Evidence</summary><ul>${evidenceLinks(gap)}</ul></details>
    </article>`).join("");
  }

  function rawRows() {
    const rows = [];
    if (["all", "remote"].includes($("dataset").value)) state.remote.forEach(record => rows.push({ type: "CHM", record }));
    Object.entries(seedSets).forEach(([type, records]) => {
      if (["all", type].includes($("dataset").value)) records.forEach(record => rows.push({ type, record }));
    });
    return rows;
  }

  function populateStatus() {
    const select = $("status");
    const old = select.value;
    const values = [...new Set(rawRows().map(row => status(row.record)))].sort();
    select.innerHTML = '<option value="">Any</option>' + values.map(value => `<option>${e(value)}</option>`).join("");
    if (values.includes(old)) select.value = old;
  }

  function renderRaw() {
    populateStatus();
    const query = $("search").value.trim().toLowerCase();
    const wanted = $("status").value;
    const records = rawRows().filter(row => (!query || JSON.stringify(row.record).toLowerCase().includes(query)) && (!wanted || status(row.record) === wanted));
    const limit = Math.max(25, Math.min(2000, Number($("limit").value) || 300));
    state.rows = records.slice(0, limit);
    $("summary").textContent = `${records.length.toLocaleString()} matching records; showing ${state.rows.length.toLocaleString()}.`;
    $("rows").innerHTML = state.rows.map((row, index) => `<tr data-index="${index}"><td><span class="badge">${e(row.type)}</span></td><td>${e(label(row.record, row.type))}</td><td class="path">${e(path(row.record))}</td><td>${badge(status(row.record))}</td><td>${typeof row.record.bytes === "number" ? row.record.bytes.toLocaleString() : ""}</td></tr>`).join("") || '<tr><td colspan="5">No matching records.</td></tr>';
    [...$("rows").querySelectorAll("tr[data-index]")].forEach(row => row.addEventListener("click", () => selectRaw(Number(row.dataset.index))));
  }

  function selectRaw(index) {
    const row = state.rows[index];
    if (!row) return;
    [...$("rows").querySelectorAll("tr")].forEach(item => item.classList.toggle("selected", item.dataset.index === String(index)));
    const entries = Object.entries(row.record).filter(([, value]) => value !== null && value !== undefined && value !== "");
    $("detail").innerHTML = `<h2>${e(label(row.record, row.type))}</h2>${entries.map(([key, value]) => `<dl><dt>${e(key)}</dt><dd>${e(typeof value === "object" ? JSON.stringify(value, null, 2) : value)}</dd></dl>`).join("")}`;
  }

  function parseJsonl(text) {
    return text.split(/\r?\n/).filter(line => line.trim()).map((line, index) => {
      try { return JSON.parse(line); }
      catch (error) { throw new Error(`Invalid JSONL at line ${index + 1}: ${error.message}`); }
    });
  }

  function showView(name) {
    document.querySelectorAll(".view").forEach(view => view.classList.toggle("active-view", view.id === `view-${name}`));
    document.querySelectorAll("#nav button").forEach(button => button.classList.toggle("active", button.dataset.view === name));
    if (name === "raw") renderRaw();
  }

  document.querySelectorAll("#nav button").forEach(button => button.addEventListener("click", () => showView(button.dataset.view)));
  ["search", "dataset", "status", "limit"].forEach(id => $(id).addEventListener("input", renderRaw));
  $("load-file").addEventListener("click", () => $("file-input").click());
  $("file-input").addEventListener("change", event => {
    const file = event.target.files[0];
    if (!file) return;
    const reader = new FileReader();
    reader.onload = () => {
      try {
        state.remote = parseJsonl(reader.result);
        state.browserIndex = null;
        $("notice").textContent = `Replaced the CHM inventory with ${state.remote.length.toLocaleString()} records from ${file.name}.`;
        renderAll();
      } catch (error) { $("notice").textContent = error.message; }
    };
    reader.readAsText(file);
  });

  $("browser-server").addEventListener("change", () => { state.browser.server = $("browser-server").value; state.browser.path = ""; renderBrowser(); });
  $("browser-search").addEventListener("input", renderBrowser);
  $("browser-unmatched").addEventListener("change", renderBrowser);

  function renderAll() {
    renderOverview();
    renderEnvironments();
    renderBrowser();
    renderRecipes();
    renderGaps();
    if ($("view-raw").classList.contains("active-view")) renderRaw();
  }

  if (state.remote.length) $("notice").textContent = `Loaded ${state.remote.length.toLocaleString()} CHM occurrences automatically. Start with Environments or Disk recipes.`;
  renderAll();
})();
