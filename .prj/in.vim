set nu
set wrap
set makeprg=./build.sh
set background=dark
noremap <F9> <Esc>:!sh $LOG_HOME/.prj/srcindex.sh <CR> <ESC>:cs kill -1 <CR> <ESC>:cs add $LOG_HOME/.prj/cscope.out <CR> 
noremap <F3> <Esc>:!sh $LOG_HOME/build.sh<CR>
ia mstr mysql_escape_string
ia clsn className()
ia ret  return

"TagList conf
let Tlist_Use_Right_Window = 1
let Tlist_Ctags_Cmd = '/usr/local/bin/exctags'
let Tlist_WinWidth = 20
noremap \tl <Esc>:Tlist<CR>
noremap \tc <Esc>:TlistClose<CR>
noremap \ts <Esc>:TlistSync<CR>
"map \n <Esc>:call DoWordComplete() <Enter>
"map \u <Esc>:call EndWordComplete() <Enter>
map \. <Esc>a$this->
