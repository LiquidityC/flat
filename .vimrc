"Inform me of lines that are too long
"match ErrorMsg '\%>110v.\+'

nnoremap <F1> :Make<cr>
nnoremap <F2> :Make check lint<cr>
nnoremap <F9> :Make clean<cr>

au FileType cpp,h,txt setl makeprg=make\ -C\ build/
