ROOT=/var/www/doxygen/0.6.1
rm -rf $ROOT
mkdir -p $ROOT
(cd ~/git/Rakkarack/rakarrack-0.6.1 && doxygen doxygen.config)
