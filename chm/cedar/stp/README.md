# Cedar STP source cache

Original Xerox source recovered from the Computer History Museum PARC archive
on 2026-07-11.  This cache is intentionally raw: keep the IFS filename and
version suffix so protocol comments and source dates remain traceable.

- `client-6.1/` is the Cedar 6.1 STP client actually present in
  `BasicCedarDorado.boot!22`.  Its `STPImpl.mesa!5` opens `PupWKS.ftp` and
  carries the basic LoaderDriver file requests over a PupStream/BSP connection.
- `server-6.1/` is the CedarChest 6.1 STP server package.  The main and helper
  modules define its release-era policy and file-name translation.
- `client-6.0/` preserves the complete compatible source split, including
  `STPOps.mesa!2` (the on-wire mark values) and `STPReplyCode.mesa!3`.
- `server-6.0/STPServerImpl.mesa!9` is the complete historical server loop.
  It is the most direct specification for the emulator's initial read-only
  `Retrieve` and `Directory` subset.

The implementation starts in `dorado/src/ethernet.c`.  It reuses the existing
Pup FTP socket 3/BSP framing: this is correct for STP, not a protocol shortcut.
`STPImpl.mesa!5` explicitly calls `PupStream.Create` with `PupWKS.ftp`, and
`STPServerImpl.mesa!9` listens on `PupTypes.ftpSoc`.

The first supported request set is deliberately read-only: version handshake,
`Retrieve`, and the metadata property list LoaderDriver needs.  Authentication,
write operations, wildcard directory enumeration, and name-service fidelity
remain future work.
