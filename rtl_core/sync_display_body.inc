// sync_display_body.inc -- the C display controller (d) into dorado_rtl_io's ddc_*
// registers: DS(name, value), DSA(name, index, value). See sync_cpu_body.inc.
    DS(ddc_statics, d->statics);
    DS(ddc_riob, d->riob);
    DS(ddc_terminal_task, d->terminal_task & 15);
    DS(ddc_ram_keep, d->ram_keep ? 1 : 0);
    DS(ddc_ram_addr, d->ram_addr);
    for (int i = 0; i < 512; i++) DSA(ddc_hram, i, d->hram[i] & 7);
    for (int i = 0; i < 16; i++) DSA(ddc_nlcb, i, d->nlcb[0][i]);
    DS(ddc_nlcb_line, d->nlcb_line);
    DS(ddc_nlcb_field_odd, d->nlcb_field_odd ? 1 : 0);
    DS(ddc_nlcb_cursor_x, d->nlcb_cursor_x);
    DS(ddc_nlcb_cursor_lo, d->nlcb_cursor_lo);
    DS(ddc_scan_line, d->scan_line);
    DS(ddc_scan_pixel, d->scan_pixel);
    DS(ddc_frame_count, d->frame_count);
    for (int c = 0; c < 2; c++) {
        DSA(ddc_next_wcb_flag, c, d->next_wcb_flag[c] ? 1 : 0);
        DSA(ddc_current_wcb_flag, c, d->current_wcb_flag[c] ? 1 : 0);
        DSA(ddc_wcb_draw_x, c, d->wcb_draw_x[c]);
        DSA(ddc_wcb_draw_y, c, d->wcb_draw_y[c]);
        const dorado_display_ddc_line *pl = &d->ddc_pending_line[c], *cl = &d->ddc_current_line[c];
        DSA(ddc_pend_valid, c, pl->valid ? 1 : 0);
        DSA(ddc_pend_odd, c, pl->odd ? 1 : 0);
        DSA(ddc_pend_line, c, pl->line);
        DSA(ddc_pend_lmarg, c, pl->lmarg);
        DSA(ddc_pend_width, c, pl->width);
        DSA(ddc_pend_ptr, c, pl->ptr);
        DSA(ddc_pend_scan, c, pl->scan);
        DSA(ddc_cur_valid, c, 0);            // the half-gathered line's words are not carried
        DSA(ddc_cur_odd, c, cl->odd ? 1 : 0);
        DSA(ddc_cur_abs_y, c, cl->absolute_y ? 1 : 0);
        DSA(ddc_cur_y, c, cl->y);
        DSA(ddc_cur_line, c, cl->line);
        DSA(ddc_cur_lmarg, c, cl->lmarg);
        DSA(ddc_cur_width, c, cl->width);
        DSA(ddc_cur_ptr, c, cl->ptr);
        DSA(ddc_cur_scan, c, cl->scan);
        DSA(ddc_cur_word_count, c, 0);
    }
    DS(ddc_fifo_a_head, d->fifo_a_head);
    DS(ddc_fifo_a_tail, d->fifo_a_tail);
    DS(ddc_fifo_b_head, d->fifo_b_head);
    DS(ddc_fifo_b_tail, d->fifo_b_tail);
    DS(ddc_raster_taskcmd, d->raster_taskcmd);
    DS(ddc_raster_lt_enabled, d->raster_lt_enabled ? 1 : 0);
    DS(ddc_raster_next_channel, d->raster_next_channel & 3);
    for (int c = 0; c < 4; c++) {
        DSA(ddc_raster_next_wt_flag, c, d->raster_next_wt_flag[c] ? 1 : 0);
        DSA(ddc_raster_current_wt_flag, c, d->raster_current_wt_flag[c] ? 1 : 0);
    }
    DS(ddc_boot_button_scanlines, d->boot_button_scanlines);
    for (int i = 0; i < 5; i++) DSA(ddc_kbd_words, i, d->keyboard_words[i]);
