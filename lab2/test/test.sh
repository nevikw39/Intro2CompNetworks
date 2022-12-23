cp test/video.mp4 bin
cd bin

./server 9999 &
./client <<EOF
127.0.0.1
9999
download video.mp4
exit
EOF
kill $!

r=$(diff video.mp4 download_video.mp4)
rm *.mp4

if $r; then
    printf "\e[32mAC!!\e[0m\n"
else
    printf "\e[31mWA!!\e[0m\n"
    exit 1
fi