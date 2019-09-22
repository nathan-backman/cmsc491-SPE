syntax on




function! CppSplit(arg)
  let cppFile = a:arg

  let hFile = split(cppFile, ".cpp")[0]
  let hFile = split(hFile, "src/")[0]
  let hFile = "src/include/" . hFile . ".h"

  execute "tabnew " . hFile
  execute "copen 5"
  execute "wincmd k"
  execute "vsplit " . cppFile
  execute "wincmd l"
  execute "vertical resize 80"
  execute "wincmd h"
endfunction
command! -nargs=1 -complete=file CppSplit call CppSplit(<f-args>)




edit Makefile
copen 5

tabnew apps/sample_driver.cpp
copen 5

CppSplit src/Engine/StreamProcessingEngine.cpp
CppSplit src/Operators/Operator.cpp
"CppSplit src/Data/Data.cpp
CppSplit src/Operators/InputSource.cpp



tabdo windo set autoindent
tabdo windo set cindent
tabdo windo set expandtab
tabdo windo set tabstop=2
tabdo windo set shiftwidth=2
tabdo windo set smarttab

tabdo windo set tw=79
tabdo windo set fo=crqwa

tabdo windo set hls
tabdo wincmd k
