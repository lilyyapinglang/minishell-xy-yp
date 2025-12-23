#include "../inc/minishell.h"

sig_atomic_t g_signal_value = 0;

void sigint_prompt(int sig_num)
{
    g_signal_value = sig_num;
    
}