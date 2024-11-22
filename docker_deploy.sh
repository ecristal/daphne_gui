sudo docker run -d --mount type=bind,source=$HOME/root/root_build,target=/home/ecristal/root/root_build --mount type=bind,source=$HOME/Documents/PHD/qt_apps,target=/home/ecristal/qt_apps --mount type=bind,source=$HOME/Qt,target=/home/ecristal/Qt --mount type=bind,source=/usr/local/include,target=/usr/local/include -w /home/ecristal/qt_apps --name ubuntu_bionic -it ubuntu:focal
sudo docker start ubuntu_bionic
sudo docker exec ubuntu_bionic dpkg --add-architecture i386
sudo docker exec ubuntu_bionic apt update 
sudo docker exec ubuntu_bionic apt install software-properties-common -y
sudo docker exec ubuntu_bionic add-apt-repository universe
sudo docker exec ubuntu_bionic apt update
sudo docker exec ubuntu_bionic apt upgrade -y 
sudo docker exec ubuntu_bionic apt install vim openssl libxcb-icccm4 libxcb-image0 libxcb-keysyms1 libxcb-render-util0 libxcb-xinerama0 libgl-dev libfontconfig1 libxrender1 build-essential libglib2.0-0 binutils p7zip-full p7zip-rar libxkbcommon-x11-0 libdbus-1-3 libcups2 libgtk-3-0 git cmake xorg-dev libx11-dev libxpm-dev libxft-dev libxext-dev gfortran patchelf libxcb-shape0 libegl1:i386 libegl-dev -y
sudo docker exec ubuntu_bionic bash -c 'source /home/ecristal/root/root_build/bin/thisroot.sh'
sudo docker exec ubuntu_bionic bash -c 'source deploy.sh'
sudo docker stop ubuntu_bionic
