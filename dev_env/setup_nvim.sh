#!/usr/bin/bash
set -euo pipefail

mkdir src
cd src

wget "https://github.com/neovim/neovim/releases/download/stable/nvim-linux64.tar.gz"

tar xvzf nvim-linux64.tar.gz

mkdir -p ~/.local
mv nvim-linux64/* ~/.local

[ "$(grep -oP "PATH=.*\.local/bin.*" ~/.bashrc)" == "" ] && echo "PATH=$HOME/.local/bin:$PATH" >> ~/.bashrc
[ "$(grep -oP "alias vi=['\"]nvim['\"]" ~/.bashrc)" == "" ] && echo "alias vi='nvim -c \"set autoindent expandtab tabstop=4 shiftwidth=4\"'" >> ~/.bashrc

git clone https://github.com/NvChad/starter ~/.config/nvim/

LANG_SERVERS=""
[ "${ENABLE_PYTHON:-true}" == "true" ] && LANG_SERVERS="$LANG_SERVERS, \"pyright\""
[ "${ENABLE_CPP:-true}" == "true" ] && LANG_SERVERS="$LANG_SERVERS, \"clangd\""

sed -i "s/servers = { \"html\", \"cssls\" }/servers = { \"html\", \"cssls\" $LANG_SERVERS }/g" $HOME/.config/nvim/lua/configs/lspconfig.lua

cd $HOME/.config/nvim/lua/plugins
head -n -1 init.lua > init_inter.lua
cat init_inter.lua > init.lua
rm init_inter.lua

var=$(cat << EOF
  {
    "cappyzawa/trim.nvim",
    config = function()
      require("trim").setup({
        ft_blocklist = {},
        patterns = {},
        trim_on_write = true,
        trim_trailing = true,
        trim_last_line = true,
        trim_first_line = true,
        highlight = true,
        highlight_bg = '#ff0000', -- or 'red'
        highlight_ctermbg = 'red',
        notifications = true,
      })
    end,
    lazy=false,
  },
EOF
)

echo "$var" >> init.lua
echo "}" >> init.lua

timeout 30 $HOME/.local/bin/nvim || true
