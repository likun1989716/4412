all:
	arm-linux-gcc  hardcontrol.c -I/usr/lib/jvm/java-7-openjdk-amd64/include/ \
	-I/work/4412/wcode/android-5.0.2/prebuilts/ndk/9/platforms/android-19/arch-arm/usr/include/ \
	-nostdlib /work/4412/wcode/android-5.0.2/prebuilts/ndk/9/platforms/android-19/arch-arm/usr/lib/libc.so \
	/work/4412/wcode/android-5.0.2/prebuilts/ndk/9/platforms/android-19/arch-arm/usr/lib/liblog.so \
	-shared -fPIC -o libhardcontrol.so
clean:
	rm -rf *.so
	rm -rf *.class
