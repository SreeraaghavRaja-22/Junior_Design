# run this file by using bash run.sh
cd ~/Junior_Design/Final_Project/Junior_Design_C/
rm -rf build 

mkdir build && cd build

cmake -DPICO_BOARD=pico2_w ..

make -j$(nproc)