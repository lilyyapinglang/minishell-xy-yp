dup2_s(prev_read_end, STDIN_FILENO, sh_ctx);
		close_s(prev_read_end, sh_ctx);