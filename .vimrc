"Inform me of lines that are too long
match ErrorMsg '\%>110v.\+'

"GF command sourcefiles
let &path="src,flat/src,"

nnoremap <F1> :Make -l<cr>
nnoremap <F2> :Make -l check lint<cr>
nnoremap <F3> :Make clean<cr>

au FileType cpp,h setl makeprg=make\ -l\ -C\ build
