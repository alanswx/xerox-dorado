#ifndef DORADO_ETHERNET_H
#define DORADO_ETHERNET_H

#include "io.h"

#include <stdint.h>
#include <stddef.h>

#define DORADO_ETHERNET_TASK_EOT  06
#define DORADO_ETHERNET_TASK_EIT  07

/* Concurrent STP/BSP connections the in-process server tracks. Cedar's
 * Installer phase has LoaderDriver's connection plus one or two from
 * FS/DFOperations live at the same time. */
#define DORADO_FTP_MAX_CONN 6
#define DORADO_ETHERNET_TIOA_DATA 015
#define DORADO_ETHERNET_TIOA_CTL  016

#define DORADO_PUP_TYPE_ETHERNET 01000
#define DORADO_PUP_TYPE_MICROCODE_BOOT_REQUEST 0264
#define DORADO_PUP_TYPE_MICROCODE_BOOT_REPLY   0265

/* EthC control register, HM §11 Figure 16 (manual bit n = C bit 15-n).
 * Bits in a field are decoded only when the field's command-enable bit
 * (low-true) is asserted. Observed microcode commands (InitialEther.mc):
 * TurnOffRx=0o170377 TurnOffTx=0o7777 TurnOnRx=0o173377 TurnOnTx=0o47777
 * SendEOP=0o67777 (TxOn+TxEOP) WaitForBOP=RxOn with RxBOP' clear. */
#define DORADO_ETHC_TXCMDENBLN   0x8000  /* TxCmdEnbl' (low-true)    */
#define DORADO_ETHC_TXON         0x4000
#define DORADO_ETHC_TXEOP        0x2000
#define DORADO_ETHC_TXCNTDWN     0x1000
#define DORADO_ETHC_RXCMDENBLN   0x0800  /* RxCmdEnbl' (low-true)    */
#define DORADO_ETHC_RXON         0x0400
#define DORADO_ETHC_RXBOPN       0x0200  /* RxBOP'                   */
#define DORADO_ETHC_TESTCMDENBLN 0x0080  /* TestCmdEnbl' (low-true)  */
#define DORADO_ETHC_NOWAKEUPS    0x0010

/* Stage-2 Alto software boot over Ethernet. The booting Alto (emulated by
 * AEmu) broadcasts a Mayday Pup to request a boot file, then receives the
 * file as an EFTP stream on socket 20. Constants from Taft's EtherBoot.asm
 * (`docs/research/ethernet-netboot/downloads/EtherBoot.mesa-1.html`) and
 * EFTPSPEC (`chm/pup/EFTPSPEC.BRAVO!1`). */
#define DORADO_PUP_TYPE_MAYDAY     0244   /* boot-file request */
#define DORADO_PUP_TYPE_BOOTDIR_REQ   0257  /* NetExec: who has boot files? */
#define DORADO_PUP_TYPE_BOOTDIR_REPLY 0260  /* server: {bfn,date,name} list */
/* Routing / time, from the Pup type assignments (IFS PupType enum, octal):
 * GatewayInformationRequest=200, Response=201; AltoTimeRequest=206,
 * Response=207. NetExec's GatewayListener probes 200B to learn its net;
 * the 201B reply's destination net teaches it (PupRoute.bcpl). */
#define DORADO_PUP_TYPE_GATEWAY_REQ    0200
#define DORADO_PUP_TYPE_GATEWAY_REPLY  0201
#define DORADO_PUP_TYPE_ALTOTIME_REQ   0206
#define DORADO_PUP_TYPE_ALTOTIME_REPLY 0207
#define DORADO_PUP_TYPE_ADDRESS_LOOKUP 0223
#define DORADO_PUP_TYPE_ADDRESS_REPLY  0224
#define DORADO_PUP_TYPE_NETDIR_LOOKUP  0220
#define DORADO_PUP_TYPE_NETDIR_REPLY   0221
#define DORADO_PUP_LOCAL_NET           01    /* the network NetExec lives on */
#define DORADO_PUP_TYPE_EFTP_DATA  030
#define DORADO_PUP_TYPE_EFTP_ACK   031
#define DORADO_PUP_TYPE_EFTP_END   032
#define DORADO_PUP_TYPE_EFTP_ABORT 033
#define DORADO_EFTP_RECEIVER_SOCKET 020
#define DORADO_EFTP_DATA_WORDS 256        /* 512-byte EFTP data section */

typedef struct dorado_ethernet {
    uint8_t local_host;
    uint8_t remote_host;

    /* Controller control/status state (HM §11 "Control Register").
     * tx_eop disables EOT wakeups; set by EOT after the last word of a
     * packet, cleared by the controller (TxGone) when the packet has
     * been sent. tx_cntdwn disables EOT wakeups until the next Pendulum
     * tick (16 us); this fake clears it immediately. */
    uint8_t rx_on;
    uint8_t tx_on;
    uint8_t tx_eop;
    uint8_t tx_cntdwn;
    uint8_t no_wakeups;
    uint8_t tx_complete;
    uint32_t rx_hold;     /* RxBOP clear (WaitForBOP): EIT wakeups stay
                           * off while the rejected packet's remaining
                           * words drain past at wire speed. Counted in
                           * wakeup-poll ticks, set by the discard. */

    /* Faithful transmit wire model (DORADO_ETH_WIRE, default OFF). When
     * EOT sets TxEOP, instead of completing the packet instantly we hold
     * tx_eop set (which suppresses the EOT "packet sent" wakeup) until the
     * transmit actually finishes: the controller defers while a receive is
     * in progress (carrier sense -- it cannot transmit while a packet is on
     * the wire), then takes wire time proportional to the packet length.
     * Only when the timer expires do we complete the packet and clear
     * tx_eop, waking EOT to post OutDone. Models why ContrAlto's OutDone is
     * delayed ~ms where the fake's is instant. */
    uint8_t  tx_pending;   /* 1 = a TxEOP packet is awaiting wire completion */
    uint32_t tx_wire_timer;/* wakeup-poll ticks remaining once the wire is free */

    /* Faithful receive wire pacing (DORADO_ETH_WIRE). ContrAlto fills its rx
     * FIFO one word per 5.4 us and the AEmu EIT microcode reads one word per
     * wakeup then Blocks, so we pace EIT wakeups: after each delivered word we
     * hold the next EIT wakeup off for one word's wire time. Without this the
     * fake hands the whole prequeued reply to the world in a few us, where the
     * real wire takes words*5.4 us -- the gap that lets games race ahead. */
    uint32_t rx_wire_timer;/* ticks until the next rx word may be delivered */

    uint16_t control_last[16];
    uint64_t control_writes[16];
    uint64_t data_writes;
    uint64_t data_reads;
    uint64_t tx_starts;
    uint64_t tx_eops;
    uint64_t tx_stops;

    uint16_t tx_words[512];
    size_t tx_count;
    uint64_t requests_seen;
    uint64_t replies_queued;
    uint16_t last_boot_offset;
    uint16_t first_control[16];

    /* Stage-2 EFTP boot server state. The transfer is LOCK-STEP per
     * EFTPSPEC: one Data packet on the wire at a time; the next is
     * sent only when the receiver's Ack for the previous arrives
     * (eth_tx_packet_done sees the Ack Pup). After the End packet's
     * Ack, one more End is sent to close the dally. */
    uint16_t last_tx_pup_type;
    uint64_t eftp_requests_seen;
    uint64_t eftp_replies_queued;
    uint16_t eftp_last_bfn;
    uint16_t eftp_dest_socket_hi;
    uint16_t eftp_dest_socket_lo;
    uint64_t bol_queued;
    uint64_t time_bcasts;     /* Alto time-reply broadcasts queued */
    char eftp_boot_path[256];
    uint16_t *eftp_words;     /* whole boot file, loaded at Mayday */
    size_t eftp_len;
    size_t eftp_pos;          /* next undelivered word */
    uint16_t eftp_seq;        /* seq of the packet currently on the wire */
    uint8_t eftp_state;       /* 0 idle, 1 data on wire, 2 End on wire,
                               * 3 dally End on wire */
    uint16_t eftp_max_seq;    /* progress: highest seq ever delivered */
    uint32_t eftp_turnaround_ticks; /* fake server delay before EIT wake */
    uint8_t eftp_wait_for_rx_arm; /* defer EFTP packet delivery until
                                   * the world has posted an input buffer */
    uint8_t eftp_rx_armed;
    uint8_t eftp_delivery_deferred;
    uint32_t eftp_resend_timer; /* EFTPSPEC sender retransmission: ticks
                                 * until the current unacked packet is
                                 * put back on the wire. Re-armed at
                                 * each delivery; counts down only once
                                 * the receiver has consumed (or lost)
                                 * the previous copy. */

    /* Minimal Pup FTP/BSP server for Interlisp-D RemoteVmemInit. Disabled
     * unless ftp_sysout_path is set. It implements just the client path:
     * RTP open to socketFTP(3), FTP Version, Retrieve of one sysout, and
     * BSP byte-stream delivery. Protocol constants are from PupRTP.decl,
     * PupBSP.decl, and FtpProt.decl in chm/altosource/. */
    char ftp_sysout_path[256];
    uint8_t ftp_enabled;
    uint8_t ftp_open;
    uint8_t ftp_pending_ack;
    uint8_t ftp_waiting_for_ack;
    uint8_t ftp_delivery_blocked; /* Cedar direct-IOCB bridge awaits repost */
    uint8_t ftp_tx_mode;
    uint8_t ftp_tx_step;
    uint8_t ftp_phase;
    uint16_t ftp_tx_in_flight; /* PupStream output fingers since last ACK */
    uint16_t ftp_conn_hi, ftp_conn_lo;
    uint16_t ftp_client_net_host;
    uint16_t ftp_client_sock_hi, ftp_client_sock_lo;
    uint16_t ftp_server_net_host;
    uint16_t ftp_server_sock_hi, ftp_server_sock_lo;
    uint32_t ftp_rx_next;
    uint32_t ftp_tx_next;
    uint32_t ftp_tx_last_end;
    uint32_t ftp_last_ack;
    uint32_t ftp_file_pos;
    uint32_t ftp_file_size;
    uint16_t ftp_client_bytes_per_pup;
    uint16_t ftp_client_pup_alloc;
    uint16_t ftp_client_byte_alloc;
    uint8_t ftp_cmd_mark;
    uint8_t ftp_cmd_data[4096];
    size_t ftp_cmd_len;
    uint64_t ftp_packets_seen;
    uint64_t ftp_packets_queued;

    /* Saved BSP/STP state for connections other than the one currently
     * loaded in the ftp_* working set above.
     *
     * Cedar keeps several STP connections to socket 3 open at once --
     * LoaderDriver's, and the ones FS/DFOperations opens for the Installer
     * -- and switches between them, so a single connection's state is not
     * enough.  Each BSP connection has its own byte-ID space (seeded from
     * its RFC connection ID), so mixing them corrupts the receive cursor.
     * The working set is context-switched by client socket: an arriving
     * packet selects its connection's slot, and a new RFC allocates one. */
    struct dorado_ftp_ctx {
        uint8_t used;
        uint8_t open, pending_ack, waiting_for_ack, tx_mode, tx_step, phase;
        uint8_t cmd_mark;
        uint16_t tx_in_flight;
        uint16_t conn_hi, conn_lo;
        uint16_t client_net_host, client_sock_hi, client_sock_lo;
        uint16_t server_net_host, server_sock_hi, server_sock_lo;
        uint16_t client_bytes_per_pup, client_pup_alloc, client_byte_alloc;
        uint32_t rx_next, tx_next, tx_last_end, last_ack;
        uint32_t file_pos, file_size;
        size_t cmd_len;
        uint8_t cmd_data[4096];
    } ftp_ctx[DORADO_FTP_MAX_CONN];
    uint8_t ftp_ctx_cur;      /* slot whose state is in the working set */
    uint8_t ftp_ctx_valid;    /* working set belongs to ftp_ctx_cur */

    uint16_t *rx_words;
    uint8_t  *rx_attention;
    size_t rx_count;
    size_t rx_pos;

    /* The emulated world's currently-posted Ethernet input-buffer size,
     * in words -- EICLOC at Alto VM 604B (Alto HW Manual Sec 7). The world
     * re-posts this constantly and frequently parks it at 0 (no receive
     * buffer) between exchanges. If we hand the receiver a packet longer
     * than the posted buffer, the AEmu input microcode posts "Input Buffer
     * Overrun" (EPLOC[0-7]=2) and exits its input loop early, derailing the
     * world. We therefore gate rx delivery on rx_count <= world_rx_words.
     * machine.c refreshes this each step once the world is loaded; it stays
     * 0xFFFF (no gate) during the Initial/EFTP microcode-boot phase, when
     * 604B is not yet a meaningful Alto cell. */
    uint16_t world_rx_words;

    char boot_110_path[256];
    char boot_111_path[256];
    char boot_112_path[256];
    char boot_113_path[256];
    char boot_114_path[256];

    /* Stage-2 boot-file directory. The real NetExec broadcasts a
     * BootDirReq (257B) to discover which boot files each server offers,
     * then lets the user boot one by name. Each entry advertises a
     * {boot file number, name, version date} tuple in our BootDirReply
     * (260B) and supplies the file to EFTP when the matching Mayday
     * (244B, ID = bfn) arrives. See ALTOBOOT.BRAVO and NetExec1.bcpl. */
    struct {
        uint16_t bfn;          /* boot file number (Mayday Pup ID low word) */
        char     name[48];     /* boot file name; must end in ".boot"       */
        char     path[256];    /* file served by EFTP for this bfn          */
        uint16_t date_hi;      /* Alto-format date (version); MUST be       */
        uint16_t date_lo;      /* nonzero or NetExec treats it as a local   */
    } bootdir[40];             /* command, not a bootable file. Sized to    */
                               /* hold every Alto game/utility in            */
                               /* chm/bootfiles/ (--boot-dir-all) plus a few */
                               /* explicit --boot-dir entries.               */
    int      bootdir_count;
    uint16_t bootdir_cursor;   /* round-robin pagination: the next entry to  */
                               /* emit in a BootDirReply. The whole table    */
                               /* can exceed one Pup (~266 data words); we    */
                               /* split it across the BootDirReqs NetExec     */
                               /* retransmits (~3 per GetDir) and its         */
                               /* InstallDir accumulates them (NetExec1.bcpl).*/
    uint64_t bootdir_replies;  /* stats: BootDirReply packets queued        */
} dorado_ethernet;

void dorado_ethernet_init(dorado_ethernet *eth);
void dorado_ethernet_free(dorado_ethernet *eth);
void dorado_ethernet_attach_to_io(dorado_ethernet *eth, dorado_io *io);
void dorado_ethernet_direct_transmit(dorado_ethernet *eth,
                                     const uint16_t *words, size_t nwords);

void dorado_ethernet_set_boot_file(dorado_ethernet *eth,
                                   uint16_t offset,
                                   const char *path);

/* Stage-2: the Alto software boot file served by the fake EFTP server in
 * response to a Mayday request (any boot-file number, for now). */
void dorado_ethernet_set_eftp_boot_file(dorado_ethernet *eth,
                                        const char *path);

/* Optional Pup FTP/BSP sysout source for Lisp RemoteVmemInit. When set,
 * the fake network directory advertises this host as an FTP server and the
 * server streams PATH in response to Retrieve. */
void dorado_ethernet_set_ftp_sysout(dorado_ethernet *eth, const char *path);

/* Serve a read-only directory tree through the same Pup FTP/STP endpoint.
 * Requested IFS names such as [Cedar]<Cedar6.1>Top>Basic.Loadees map below
 * ROOT as Cedar6.1/Top/Basic.Loadees. */
void dorado_ethernet_set_ftp_root(dorado_ethernet *eth, const char *root);

/* True while an STP file body or its source-defined Yes/text/EOC trailer is
 * being delivered.  The Cedar CSB bridge uses this to apply receive repost
 * backpressure without delaying the preceding plist negotiation. */
int dorado_ethernet_ftp_file_delivery_active(const dorado_ethernet *eth);

/* Optional Stage-2 delivery gate for clients such as the Cedar/Pilot germ
 * that post Ethernet input IOCBs through a controller status block. */
void dorado_ethernet_set_eftp_rx_armed(dorado_ethernet *eth, int armed);

/* Stage-2: register a boot file in the directory the fake boot server
 * advertises to NetExec. `name` must end in ".boot"; `bfn` is the boot
 * file number NetExec puts in its Mayday Pup ID when the user boots this
 * file by name. The file at `path` is then EFTP-served for that bfn. */
void dorado_ethernet_add_boot_dir(dorado_ethernet *eth, uint16_t bfn,
                                  const char *name, const char *path);

/* Stage-2: scan `dir` for `*.boot` files and register every one that is an
 * Alto B-format image (big-endian word 0 = 0o405, runnable on the AEmu Alto
 * emulator). Mesa-format files (word 0 = 0o345) are skipped -- they need the
 * AltoMesaDorado path that isn't up yet. Each registered file gets an
 * advertised name derived from its filename (version suffix stripped, the
 * extension normalized to lower-case ".boot") and a unique auto-assigned
 * boot file number that avoids the microcode boot offsets (0o110-0o114) and
 * any bfn already in the directory. Returns the number of files registered.
 * Files whose derived name is already present are skipped, so explicit
 * dorado_ethernet_add_boot_dir() entries take precedence. */
int dorado_ethernet_add_boot_dir_all(dorado_ethernet *eth, const char *dir);

/* Returns a wakeup bitmask for EOT/EIT based on current controller
 * state. Non-const: ticks the post-WaitForBOP drain hold. */
uint16_t dorado_ethernet_wakeup_mask(dorado_ethernet *eth);

/* Stage-2: deliver one broadcast breath-of-life packet (Alto boot
 * loader, ether type 602B) if the receiver is on and idle. Returns 1
 * if a packet was queued. Call periodically to model a boot server's
 * rebroadcast. */
int dorado_ethernet_breath_of_life(dorado_ethernet *eth);

/* Stage-2: broadcast an Alto time-protocol reply (Pup type 201B), as
 * a real network's time server would. NetExec's startup raw-listens
 * for one before its contexts run. */
int dorado_ethernet_time_broadcast(dorado_ethernet *eth);

#endif
