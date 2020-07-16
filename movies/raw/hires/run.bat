for %%f in (*.*) do ffmpeg -i %%f -c:v libx264 -s 720x480 -pix_fmt yuv420p %%~nf.mp4
pause