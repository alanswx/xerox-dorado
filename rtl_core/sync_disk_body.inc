// sync_disk_body.inc -- the C Trident controller (CD) into dorado_rtl_io's disk_*
// registers: DS(name, value), DSA(name, index, value). See sync_cpu_body.inc.
    DS(disk_selected_drive, CD->selected_drive & 3);
    for (int i = 0; i < 4; i++) {
        const dorado_disk_drive *d = &CD->drive[i];
        const dorado_disk_geometry *g = d->pack ? &d->pack->geometry : nullptr;
        DSA(disk_drv_online, i, d->online ? 1 : 0);
        DSA(disk_drv_selected, i, d->selected ? 1 : 0);
        DSA(disk_drv_read_only, i, d->read_only ? 1 : 0);
        DSA(disk_drv_head_ovfl, i, d->head_overflow ? 1 : 0);
        DSA(disk_drv_media, i, d->pack ? 1 : 0);
        DSA(disk_drv_cyls, i, g ? (g->cylinders & 0xFFF) : 0);
        if (g) {
            DSA(disk_drv_heads, i, g->heads & 0x3F);
            DSA(disk_drv_sectors, i, g->sectors & 0x7F);
            DSA(disk_drv_hdr_words, i, dorado_disk_geom_header_words(g) & 0xF);
            DSA(disk_drv_lbl_words, i, dorado_disk_geom_label_words(g) & 0x1F);
            DSA(disk_drv_data_words, i, dorado_disk_geom_data_words(g) & 0x7FF);
            DSA(disk_drv_is_diablo, i, (g->sectors == 29 && dorado_disk_geom_header_words(g) == 2 &&
                                            dorado_disk_geom_label_words(g) == 8 &&
                                            dorado_disk_geom_data_words(g) == 256) ? 1 : 0);
        }
        DSA(disk_drv_sectors_per_rev, i, d->sectors_per_revolution & 0x7F);
        DSA(disk_drv_subsector_cnt, i, d->subsector_count & 0x3F);
        DSA(disk_drv_cur_cyl, i, d->cur_cyl & 0xFFF);
        DSA(disk_drv_cur_head, i, d->cur_head & 0x3F);
        DSA(disk_drv_cur_sector, i, d->cur_sector & 0x7F);
        DSA(disk_drv_seek_in_prog, i, d->seek_in_progress);
        DSA(disk_drv_index_pulse, i, d->index_pulse ? 1 : 0);
    }
    DS(disk_control, CD->control);
    DS(disk_tag, CD->tag);
    DS(disk_muff_addr, CD->muff_addr);
    for (int i = 0; i < DORADO_DISK_FORMAT_RAM_WORDS; i++) DSA(disk_format_ram, i, CD->format_ram[i]);
    DS(disk_format_ram_addr, CD->format_ram_addr & 0xF);
    DS(disk_enable_run, CD->enable_run);
    DS(disk_debug_mode, CD->debug_mode);
    DS(disk_block_till_index, CD->block_till_index);
    DS(disk_active, CD->active);
    DS(disk_xfer_pending, CD->xfer_pending);
    DS(disk_restore_pending, CD->restore_pending);
    DS(disk_transfer_restore, CD->transfer_restore);
    DS(disk_restore_hdr_check, CD->restore_header_check);
    DS(disk_index_tw, CD->index_tw);
    DS(disk_sector_tw, CD->sector_tw);
    DS(disk_tag_tw, CD->tag_tw);
    DS(disk_rd_fifo_tw, CD->rd_fifo_tw);
    DS(disk_wr_fifo_tw, CD->wr_fifo_tw);
    DS(disk_compare_err, CD->compare_err);
    DS(disk_read_data_err, CD->read_data_err);
    DS(disk_fifo_underflow, CD->fifo_underflow);
    DS(disk_fifo_overflow, CD->fifo_overflow);
    for (int i = 0; i < DORADO_DISK_FIFO_WORDS; i++) DSA(disk_fifo, i, CD->fifo[i]);
    DS(disk_fifo_head, CD->fifo_head);
    DS(disk_fifo_tail, CD->fifo_tail);
    DS(disk_fifo_count, CD->fifo_count);
    DS(disk_read_stream_active, 0);
    DS(disk_read_block_framing, CD->read_block_framing);
    DS(disk_read_stream_index, CD->read_stream_index);
    DS(disk_write_stream_active, 0);
    DS(disk_write_block_framing, CD->write_block_framing);
    DS(disk_write_stream_index, CD->write_stream_index);
    DS(disk_current_block, CD->current_block & 7);
    DS(disk_current_block_op, CD->current_block_op & 3);
    DS(disk_current_block_words, CD->current_block_words);
    DS(disk_current_block_pos, CD->current_block_pos);
    DS(disk_current_block_prefix, CD->current_block_prefix);
    DS(disk_current_block_trailer, CD->current_block_trailer & 0xF);
    // dumping a checkpoint for the board: unarmed, so the RTL arms on its own
    // microinstruction count at its real 3600 RPM (C's clock is bb.cycles)
