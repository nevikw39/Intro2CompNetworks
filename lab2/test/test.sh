cp test/video.mp4 bin
cp include/lab2.h bin
cd bin

killall server

./server 9999 &
sleep 0.1
time ./client <<EOF
127.0.0.1
9999
download video.mp4
download video.mp4
download lab2.h
download server
download client
exit
EOF
kill $!

r=$(diff video.mp4 download_video.mp4 && diff lab2.h download_lab2.h)
rm *.mp4 *.h download*

if $r; then
    printf "\e[32mAC!!\e[0m\n"
else
    printf "\e[31mWA!!\e[0m\n"
    exit 1
fi