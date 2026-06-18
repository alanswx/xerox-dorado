/*
 * pdi.h — PARC Disk Image (PDI) loader for Pilot/Cedar volumes.
 *
 * PDI is the label-bearing disk-image container used by the project's disk
 * tooling ("Rusty Backup"). A Pilot family image (fsFamily == 2) stores each
 * disk page as an independently addressed LABEL + DATA pair (the HEADER /
 * disk-address field is omitted; the page's address is its page number / VDA).
 * The label holds the entire filesystem structure (file id, file page,
 * attributes, boot-chain link); a data-only image cannot represent the FS.
 * See docs/cedar-disk-boot-plan.md and CedarDisk/PARC_PILOT_FORMAT.md.
 *
 * On-disk container (big-endian 16-bit words throughout):
 *   byte 0    "PARCDISK"  (8-byte magic)
 *   word 4    version            (= 1)
 *   word 5    fsFamily           (= 2 for Pilot/Cedar)
 *   word 8    pageCount          (e.g. 1400)
 *   word 12   labelBytes         (= 20  -> 10 words)
 *   word 13   dataBytes          (= 512 -> 256 words)
 *   ...
 *   byte 512  first sector. Each sector = labelBytes label + dataBytes data,
 *             pageCount of them, contiguous.
 *
 * The Pilot page model (PARC_PILOT_FORMAT.md §1, §2):
 *   label  = 10 words (sizeLabel = 12B): fileID(5w), filePage lo/hi(2w),
 *            attributes(1w), dontCare/boot-chain link(2w).
 *   data   = 256 words = 512 bytes (DiskFace.wordsPerPage).
 */
#ifndef DORADO_PDI_H
#define DORADO_PDI_H

#include <stdint.h>
#include <stddef.h>

#define DORADO_PDI_MAGIC        "PARCDISK"
#define DORADO_PDI_FS_PILOT     2
#define DORADO_PDI_HEADER_BYTES 512

/* Pilot page geometry (PARC_PILOT_FORMAT.md §1). */
#define DORADO_PILOT_LABEL_WORDS 10   /* 20-byte label */
#define DORADO_PILOT_DATA_WORDS  256  /* 512-byte page */

typedef struct dorado_pdi {
    uint16_t version;
    uint16_t fs_family;     /* 2 = Pilot */
    uint32_t page_count;
    uint16_t label_words;   /* labelBytes/2 */
    uint16_t data_words;    /* dataBytes/2  */
    uint16_t *labels;       /* page_count * label_words, host (decoded) words */
    uint16_t *data;         /* page_count * data_words,  host (decoded) words */
} dorado_pdi;

/* Load a PDI file (decompressed .pdi; not .gz). Returns 0 on success and fills
 * *out (caller owns and must dorado_pdi_free). Returns non-zero on error and
 * writes a message to err (if non-NULL, errlen bytes). */
int dorado_pdi_load(const char *path, dorado_pdi *out, char *err, size_t errlen);

void dorado_pdi_free(dorado_pdi *p);

/* Page accessors. Return NULL if page is out of range. */
const uint16_t *dorado_pdi_page_data(const dorado_pdi *p, uint32_t page);
const uint16_t *dorado_pdi_page_label(const dorado_pdi *p, uint32_t page);

/* Label field accessors (Pilot label, PARC_PILOT_FORMAT.md §2). */
uint32_t dorado_pdi_label_fileid(const uint16_t *label);  /* nucleus 32-bit FileID */
uint32_t dorado_pdi_label_filepage(const uint16_t *label); /* file-relative page */
uint16_t dorado_pdi_label_attr(const uint16_t *label);     /* word 7 */

/* Read a whole file (all pages whose label FileID == file_id) in file-page
 * order into out_words (data words only; labels dropped). Returns the number
 * of pages read, or -1 on error. Writes at most max_words; if max_words is
 * too small it still returns the true page count but only fills what fits. */
long dorado_pdi_read_file(const dorado_pdi *p, uint32_t file_id,
                          uint16_t *out_words, size_t max_words);

/* Pilot page-integrity checksum (PARC_PILOT_FORMAT.md §8): 16-bit
 * ones-complement sum with a left rotate after each word; 0xFFFF normalizes
 * to 0. Computed over the page's first 255 words; stored at word 255 on root
 * and marker pages. */
uint16_t dorado_pilot_checksum(const uint16_t *words, int n);

#endif /* DORADO_PDI_H */
