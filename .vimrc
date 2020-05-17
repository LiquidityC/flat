"Inform me of lines that are too long
"match ErrorMsg '\%>110v.\+'

nnoremap <F1> :Make<cr>
nnoremap <F2> :Make clean<cr>
nnoremap <F3> :Make check lint<cr>

au BufEnter *.h :setlocal filetype=cpp

let g:syntastic_c_include_dirs = [ 'build', '/usr/include/SDL2' ]
