"Inform me of lines that are too long
match ErrorMsg '\%>110v.\+'

"GF command sourcefiles
let &path="src,flat/src,"

nnoremap <F1> :Make -l<cr>
nnoremap <F2> :Make -l check_flat lint_flat<cr>
nnoremap <F9> :Make clean<cr>

au FileType cpp,h setl makeprg=make\ -l\ --no-print-dir\ -C\ build
