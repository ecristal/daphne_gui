export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ecristal/Qt/5.15.2/gcc_64/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ecristal/root/root_build/lib
#export PATH=$PATH:/home/ecristal/Qt/5.12.2/gcc_64/bin
rm -rf ./deploy/*
rm deploy.7z
rm -rf ./build-daphne_gui-Desktop_Qt_5_15_2_GCC_64bit-Release/*
cd ./build-daphne_gui-Desktop_Qt_5_15_2_GCC_64bit-Release
/home/ecristal/Qt/5.15.2/gcc_64/bin/qmake /home/ecristal/qt_apps/daphne_gui/daphne_gui.pro -spec linux-g++ CONFIG+=qtquickcompiler && /usr/bin/make qmake_all
make -j4
cd ..
cp ./build-daphne_gui-Desktop_Qt_5_15_2_GCC_64bit-Release/daphne_gui ./deploy
cd deploy
/home/ecristal/Qt/Tools/linuxdeployqt/linuxdeployqt daphne_gui -verbose=2 -qmake=/home/ecristal/Qt/5.15.2/gcc_64/bin/qmake -extra-plugins=iconengines,platformthemes/libqgtk3.so
cd ..
#zip -r deploy.zip deploy
7za a -t7z -m0=lzma2 -mx=9 -mfb=64 -md=32m -ms=on deploy.7z deploy
rm -rf ./build-daphne_gui-Desktop_Qt_5_15_2_GCC_64bit-Release/*
