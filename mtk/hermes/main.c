#define LOG_TAG "hermes"

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cutils/properties.h>
#include <cutils/log.h>
#include <private/android_filesystem_config.h>


static void s_mknod(const char *pathname, mode_t type, unsigned int maj, unsigned int min)
{
	mknod(pathname, type, makedev(maj, min));
}


static void s_mkdirp(const char *pathname)
{
	char buf[PATH_MAX];
	char currpath[PATH_MAX], *pathpiece;
	struct stat st;
	int ret;

	// buffer for strtok
	// hopefully the strings are not too long
	strncpy(buf, pathname, PATH_MAX);

	// reset path
	strcpy(currpath, "");
	// create the pieces of the path along the way
	pathpiece = strtok(buf, "/");
	if(buf[0] == '/') {
		// prepend / if needed
		strcat(currpath, "/");
	}
	while(pathpiece != NULL) {
		if(strlen(currpath) + strlen(pathpiece) + 2/*NUL and slash*/ > PATH_MAX) {
			ALOGE("Invalid path specified: too long");
			return;  // return 1;
		}
		strcat(currpath, pathpiece);
		strcat(currpath, "/");
		if(stat(currpath, &st) != 0) {
			ret = mkdir(currpath, 0777);
			if(ret < 0) {
				ALOGE("mkdir failed for %s, %s\n", currpath, strerror(errno));
				return;  // return ret;
			}
		}
		pathpiece = strtok(NULL, "/");
	}
}


static void s_echo(const char *pathname, const void *buf, size_t len)
{
	int fd;
	int remaining;
	int ret;

	fd = open(pathname, O_WRONLY);
	if (fd < 0) {
		ALOGE("echo: failed to open file %s: %s", pathname, strerror(errno));
		return;
	}

	remaining = len;
	while (remaining) {
		ret = write(fd, buf, remaining);
		if (ret < 0) {
			ALOGE("echo: write failed: %s", strerror(errno));
			goto bail;
		}
		remaining -= ret;
	}

bail:
	close(fd);
}


int main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
	ALOGI("starting");

	chown("/sys/devices/platform/Accdet_Driver/driver/accdet_call_state", AID_ROOT, AID_RADIO);

	// /dev/ttyMT2 for Connectivity BT/FM/GPS usage
	chmod("/dev/ttyMT2", 0660);
	chown("/dev/ttyMT2", AID_SYSTEM, AID_SYSTEM);

	// SMB
	chown("/proc/smb/ScreenComm", AID_SYSTEM, AID_SYSTEM);
	chmod("/proc/smb/ScreenComm", 0660);


	s_mknod("/dev/stpwmt", S_IFCHR, 190, 0);
	s_mknod("/dev/stpgps", S_IFCHR, 191, 0);
	s_mknod("/dev/stpbt", S_IFCHR, 192, 0);

	chmod("/dev/stpwmt", 0660);
	chown("/dev/stpwmt", AID_SYSTEM, AID_SYSTEM);

	chmod("/dev/wmtdetect", 0660);
	chown("/dev/wmtdetect", AID_SYSTEM, AID_SYSTEM);

	s_mknod("/dev/wmtWifi", S_IFCHR, 153, 0);
	chmod("/dev/wmtWifi", 0660);
	chown("/dev/wmtWifi", AID_SYSTEM, AID_SYSTEM);
	
     // BATTERY proc
	chown("/proc/mtk_battery_cmd/current_cmd", AID_RADIO, AID_SYSTEM);
	chown("/proc/mtk_battery_cmd/discharging_cmd", AID_RADIO, AID_SYSTEM);
	chown("/proc/mtk_battery_cmd/battery_cmd", AID_RADIO, AID_SYSTEM);
	chmod("/proc/mtk_battery_cmd/current_cmd", 0664);
	chmod("/proc/mtk_battery_cmd/discharging_cmd", 0664);
	chmod("/proc/mtk_battery_cmd/battery_cmd", 0664);

	chmod("/dev/kmsg", 0660);
	chmod("/dev/ion", 0660);


	// Camera
	chmod("/dev/GAF001AF", 0660);
	chown("/dev/GAF001AF", AID_SYSTEM, AID_CAMERA);

	chmod("/dev/DW9714AF", 0660);
	chown("/dev/DW9714AF", AID_SYSTEM, AID_CAMERA);

	chmod("/dev/DW9761BAF", 0660);
	chown("/dev/DW9761BAF", AID_SYSTEM, AID_CAMERA);

	chmod("/dev/AD5820AF", 0660);
	chown("/dev/AD5820AF", AID_SYSTEM, AID_CAMERA);

	chmod("/dev/BU64745GWZAF", 0660);
	chown("/dev/BU64745GWZAF", AID_SYSTEM, AID_CAMERA);

	chmod("/dev/LC898212AF", 0660);
	chown("/dev/LC898212AF", AID_SYSTEM, AID_CAMERA);

	chmod("/dev/LC898212XD", 0660);
	chown("/dev/LC898212XD", AID_SYSTEM, AID_CAMERA);

   chmod("/dev/camera-sysram", 0660);
   chmod("/dev/camera-isp", 0660);
   chmod("/dev/camera-fdvt", 0660);
   chmod("/dev/camera-pipemgr", 0660);
   chmod("/dev/kd_camera_hw", 0660);
   chmod("/dev/kd_camera_flashlight", 0660);
   chmod("/dev/kd_camera_hw_bus2", 0660);
   chmod("/dev/FM50AF", 0660);
   chmod("/dev/CAM_CAL_DRV", 0660);
   chown("/dev/camera-sysram", AID_SYSTEM, AID_CAMERA);
   chown("/dev/camera-isp", AID_SYSTEM, AID_CAMERA);
   chown("/dev/camera-fdvt", AID_SYSTEM, AID_CAMERA);
   chown("/dev/camera-pipemgr", AID_SYSTEM, AID_CAMERA);
   chown("/dev/kd_camera_hw", AID_SYSTEM, AID_CAMERA);
   chown("/dev/kd_camera_flashlight", AID_SYSTEM, AID_CAMERA);
   chown("/dev/kd_camera_hw_bus2", AID_SYSTEM, AID_CAMERA);
   chown("/dev/FM50AF", AID_SYSTEM, AID_CAMERA);
   chown("/dev/CAM_CAL_DRV", AID_SYSTEM, AID_CAMERA);

	// FlashLight
	chmod("/sys/class/flashlightdrv/kd_camera_flashlight/flash1", 0660);
	chown("/sys/class/flashlightdrv/kd_camera_flashlight/flash1", AID_MEDIA, AID_SYSTEM);
	chmod("/sys/class/flashlightdrv/kd_camera_flashlight/flash2", 0660);
	chown("/sys/class/flashlightdrv/kd_camera_flashlight/flash2", AID_MEDIA, AID_SYSTEM);

	// BT
//	chmod("/dev/stpbt", 0660);
//	chown("/dev/stpbt", AID_BLUETOOTH, AID_RADIO);

	// GPS
	chown("/dev/stpgps", AID_GPS, AID_GPS);
	chown("/sys/class/gpsdrv/gps/pwrctl", AID_GPS, AID_GPS);
	chown("/sys/class/gpsdrv/gps/suspend", AID_GPS, AID_GPS);
	chown("/sys/class/gpsdrv/gps/state", AID_GPS, AID_GPS);
	chown("/sys/class/gpsdrv/gps/pwrsave", AID_GPS, AID_GPS);
	chown("/sys/class/gpsdrv/gps/status", AID_GPS, AID_GPS);
//	chmod("/dev/stpgps", 0660);

	// WiFi
	s_mkdirp("/data/misc/wifi");
	chmod("/data/misc/wifi", 0770);
	chown("/data/misc/wifi", AID_WIFI, AID_WIFI);
	s_mkdirp("/data/misc/wifi/sockets");
	chmod("/data/misc/wifi/sockets", 0770);
	chown("/data/misc/wifi/sockets", AID_WIFI, AID_WIFI);
	s_mkdirp("/data/misc/wpa_supplicant");
	chmod("/data/misc/wpa_supplicant", 0770);
	chown("/data/misc/wpa_supplicant", AID_WIFI, AID_WIFI);
	chown("/data/misc/wifi", AID_WIFI, AID_WIFI);

	// ANT
	chmod("/dev/stpant", 0660);
	chown("/dev/stpant", AID_SYSTEM, AID_SYSTEM);

	// Char device for BT 3.0 HS
	s_mknod("/dev/ampc0", S_IFCHR, 151, 0);
	chown("/dev/ampc0", AID_BLUETOOTH, AID_BLUETOOTH);
	chmod("/dev/ampc0", 0660);

    // consumerir
	chmod("/dev/consumerir", 0660);
	chown("/dev/consumerir", AID_SYSTEM, AID_SYSTEM);

    // selinux
	chmod("/sys/fs/selinux/policy", 0664);

    // vol key wake system
	chmod("/sys/bus/platform/drivers/mtk-kpd/kpd_call_state", 0660);
	chown("/sys/bus/platform/drivers/mtk-kpd/kpd_call_state", AID_SYSTEM, AID_SYSTEM);

	// backlight
	chmod("/sys/class/leds/flashlight/brightness", 0664);
	chown("/sys/class/leds/flashlight/brightness", AID_SYSTEM, AID_RADIO);
	chmod("/sys/class/leds/lcd-backlight/brightness", 0664);
	chown("/sys/class/leds/lcd-backlight/brightness", AID_SYSTEM, AID_RADIO);

	// leds
	chown("/sys/class/leds/blue/trigger", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/leds/green/trigger", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/leds/red/trigger", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/class/leds/lcd-backlight/trigger", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/leds/button-backlight/trigger", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/leds/keyboard-backlight/trigger", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/leds/jogball-backlight/trigger", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/class/leds/yellow/brightness", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/class/leds/white/brightness", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/class/leds/cyan/brightness", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/class/leds/violet/brightness", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/class/leds/yellow/trigger", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/class/leds/white/trigger", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/class/leds/cyan/trigger", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/class/leds/violet/trigger", AID_SYSTEM, AID_SYSTEM);

	// Permissions for power saving mode
	chown("/sys/power/tuxonice", AID_RADIO, AID_SYSTEM);
   chown("/sys/power/tuxonice/do_hibernate", AID_RADIO, AID_SYSTEM);
	chown("/sys/power/vcorefs/pwr_ctrl", AID_RADIO, AID_SYSTEM);

	chown("/sys/android_power/state", AID_RADIO, AID_SYSTEM);
   chown("/sys/android_power/request_state", AID_RADIO, AID_SYSTEM);
	chown("/sys/android_power/acquire_full_wake_lock", AID_RADIO, AID_SYSTEM);

	chown("/sys/android_power/acquire_partial_wake_lock", AID_RADIO, AID_SYSTEM);
   chown("/sys/android_power/release_wake_lock", AID_RADIO, AID_SYSTEM);


   chmod("/sys/power/autosleep", 0660);
   chmod("/sys/power/tuxonice", 0770);
   chmod("/sys/power/tuxonice/do_hibernate", 0220);
	chown("/sys/power/sb_state", AID_SYSTEM, AID_SYSTEM);

	chown("/sys/power/autosleep", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/power/state", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/power/wakeup_count", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/power/wake_lock", AID_RADIO, AID_SYSTEM);
	chown("/sys/power/wake_unlock", AID_RADIO, AID_SYSTEM);

   chmod("/sys/power/sb_state", 0660);
   chmod("/sys/power/state", 0660);
   chmod("/sys/power/wake_lock", 0660);
   chmod("/sys/power/wake_unlock", 0660);

	// Sensor
	chmod("/dev/hwmsensor", 0660);
	chown("/dev/hwmsensor", AID_SYSTEM, AID_SYSTEM);

	chmod("/dev/msensor", 0660);
	chown("/dev/msensor",  AID_SYSTEM, AID_SYSTEM);

	chmod("/dev/gsensor", 0660);
	chown("/dev/gsensor", AID_SYSTEM, AID_SYSTEM);

	chmod("/dev/als_ps", 0660);
	chown("/dev/als_ps", AID_RADIO, AID_SYSTEM);

   chmod("/dev/gyroscope", 0660);
	chown("/dev/gyroscope", AID_SYSTEM, AID_SYSTEM);

	chmod("/dev/aal_als", 0660);
	chown("/dev/aal_als", AID_SYSTEM, AID_SYSTEM);

   chmod("/sys/devices/platform/msensor/driver/cpsopmode", 0660);
	chmod("/sys/devices/platform/msensor/driver/cpsreptxy", 0660);
	chmod("/sys/devices/platform/msensor/driver/cpsreptz", 0660);	
	chmod("/sys/devices/platform/gsensor/driver/cpsopmode", 0660);
	chmod("/sys/devices/platform/gsensor/driver/cpsrange", 0660);
    
	chown("/sys/devices/platform/msensor/driver/cpsopmode", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/devices/platform/msensor/driver/cpsreptxy", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/devices/platform/msensor/driver/cpsreptz", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/devices/platform/gsensor/driver/cpsopmode", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/devices/platform/gsensor/driver/cpsrange", AID_SYSTEM, AID_SYSTEM);

	chmod("/sys/class/misc/m_acc_misc/accenablenodata", 0660);
	chmod("/sys/class/misc/m_acc_misc/accactive", 0660);
	chmod("/sys/class/misc/m_acc_misc/accdelay", 0660);
	chmod("/sys/class/misc/m_acc_misc/accbatch", 0660);
	chmod("/sys/class/misc/m_acc_misc/accflush", 0660);
 	chown("/sys/class/misc/m_acc_misc/accenablenodata", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_acc_misc/accactive", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_acc_misc/accdelay", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_acc_misc/accbatch", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_acc_misc/accflush", AID_SYSTEM, AID_SYSTEM);

   chmod("/sys/class/misc/m_mag_misc/magactive", 0660);
   chmod("/sys/class/misc/m_mag_misc/magdelay", 0660);
   chmod("/sys/class/misc/m_mag_misc/magbatch", 0660);
   chmod("/sys/class/misc/m_mag_misc/magflush", 0660);
   chmod("/sys/class/misc/m_mag_misc/magoactive", 0660);
   chmod("/sys/class/misc/m_mag_misc/magodelay", 0660);
   chmod("/sys/class/misc/m_mag_misc/magobatch", 0660);
   chmod("/sys/class/misc/m_mag_misc/magoflush", 0660);

	chown("/sys/class/misc/m_mag_misc/magactive", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_mag_misc/magdelay", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_mag_misc/magbatch", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_mag_misc/magflush", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_mag_misc/magoactive", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_mag_misc/magodelay", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_mag_misc/magobatch", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_mag_misc/magoflush", AID_SYSTEM, AID_SYSTEM);

   chmod("/sys/class/misc/m_batch_misc/batchactive", 0660);
   chmod("/sys/class/misc/m_batch_misc/batchdelay", 0660);
   chmod("/sys/class/misc/m_batch_misc/batchbatch", 0660);
   chmod("/sys/class/misc/m_batch_misc/batchflush", 0660);

	chown("/sys/class/misc/m_batch_misc/batchactive", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_batch_misc/batchdelay", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_batch_misc/batchbatch", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_batch_misc/batchflush", AID_SYSTEM, AID_SYSTEM);

   chmod("/sys/class/misc/m_gyro_misc/gyroenablenodata", 0660);
   chmod("/sys/class/misc/m_gyro_misc/gyroactive", 0660);
   chmod("/sys/class/misc/m_gyro_misc/gyrodelay", 0660);
   chmod("/sys/class/misc/m_gyro_misc/gyrobatch", 0660);
   chmod("/sys/class/misc/m_gyro_misc/gyroflush", 0660);

	chown("/sys/class/misc/m_gyro_misc/gyroenablenodata", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_gyro_misc/gyroactive", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_gyro_misc/gyrodelay", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_gyro_misc/gyrobatch", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_gyro_misc/gyroflush", AID_SYSTEM, AID_SYSTEM);

   chmod("/sys/class/misc/m_alsps_misc/alsactive", 0660);
   chmod("/sys/class/misc/m_alsps_misc/alsdelay", 0660);
   chmod("/sys/class/misc/m_alsps_misc/alsbatch", 0660);
   chmod("/sys/class/misc/m_alsps_misc/alsflush", 0660);
   chmod("/sys/class/misc/m_alsps_misc/psactive", 0660);
   chmod("/sys/class/misc/m_alsps_misc/psdelay", 0660);
   chmod("/sys/class/misc/m_alsps_misc/psbatch", 0660);
   chmod("/sys/class/misc/m_alsps_misc/psflush", 0660);

	chown("/sys/class/misc/m_alsps_misc/alsactive", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_alsps_misc/alsdelay", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_alsps_misc/alsbatch", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_alsps_misc/alsflush", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_alsps_misc/psactive", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_alsps_misc/psdelay", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_alsps_misc/psbatch", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_alsps_misc/psflush", AID_SYSTEM, AID_SYSTEM);

   chmod("/sys/class/misc/m_baro_misc/baroenablenodata", 0660);
   chmod("/sys/class/misc/m_baro_misc/baroactive", 0660);
   chmod("/sys/class/misc/m_baro_misc/barodelay", 0660);
   chmod("/sys/class/misc/m_baro_misc/barobatch", 0660);
   chmod("/sys/class/misc/m_baro_misc/baroflush", 0660);

	chown("/sys/class/misc/m_baro_misc/baroenablenodata", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_baro_misc/baroactive", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_baro_misc/barodelay", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_baro_misc/barobatch", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/misc/m_baro_misc/baroflush", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/devices/platform/Accdet_Driver/driver/accdet_call_state", AID_ROOT, AID_SYSTEM);

	//add for gyro 
   chmod("/sys/bus/platform/drivers/gyroscope/rawdata", 0660);
   chmod("/sys/bus/platform/drivers/gyroscope/datarate ", 0660);
   chmod("/sys/bus/platform/drivers/gyroscope/range", 0660);
   chmod("/sys/bus/platform/drivers/gyroscope/powermode", 0660);
   chmod("/sys/bus/platform/drivers/gyroscope/fifo_mode", 0660); 
   chmod("/sys/bus/platform/drivers/gyroscope/fifo_data_frame", 0660); 
   chmod("/sys/bus/platform/drivers/gyroscope/fifo_framecount", 0660);

   chown("/sys/bus/platform/drivers/gyroscope/rawdata", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/bus/platform/drivers/gyroscope/datarate", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/bus/platform/drivers/gyroscope/range", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/bus/platform/drivers/gyroscope/powermode", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/bus/platform/drivers/gyroscope/fifo_mode", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/bus/platform/drivers/gyroscope/fifo_data_frame", AID_SYSTEM, AID_SYSTEM);
   chown("/sys/bus/platform/drivers/gyroscope/fifo_framecount", AID_SYSTEM, AID_SYSTEM);

   // ion device
   chmod("/dev/ion", 0660); 
   chmod("/sys/bus/platform/drivers/gsensor/cpsdata", 0660); 
   chmod("/sys/bus/platform/drivers/gsensor/cpsrange", 0660); 
   chmod("/sys/bus/platform/drivers/gsensor/cpsbandwidth", 0660); 
   chmod("/sys/bus/platform/drivers/gsensor/cpsopmode", 0660); 
   chmod("/sys/bus/platform/drivers/gsensor/fifo_mode", 0660); 
   chmod("/sys/bus/platform/drivers/gsensor/fifo_data_frame", 0660); 
   chmod("/sys/bus/platform/drivers/gsensor/fifo_framecount", 0660); 

   chown("/sys/bus/platform/drivers/gsensor/cpsdata", AID_SYSTEM, AID_SYSTEM); 
   chown("/sys/bus/platform/drivers/gsensor/cpsrange", AID_SYSTEM, AID_SYSTEM); 
   chown("/sys/bus/platform/drivers/gsensor/cpsbandwidth", AID_SYSTEM, AID_SYSTEM); 
   chown("/sys/bus/platform/drivers/gsensor/cpsopmode", AID_SYSTEM, AID_SYSTEM); 
   chown("/sys/bus/platform/drivers/gsensor/fifo_mode", AID_SYSTEM, AID_SYSTEM); 
   chown("/sys/bus/platform/drivers/gsensor/fifo_data_frame", AID_SYSTEM, AID_SYSTEM); 
   chown("/sys/bus/platform/drivers/gsensor/fifo_framecount", AID_SYSTEM, AID_SYSTEM); 

	//add for mag 
   chmod("/sys/bus/platform/drivers/msensor/rawdata", 0660); 
   chown("/sys/bus/platform/drivers/msensor/rawdata", AID_SYSTEM, AID_SYSTEM); 
   chmod("/sys/bus/platform/drivers/msensor/daemon", 0660);
   chown("/sys/bus/platform/drivers/msensor/daemon", AID_SYSTEM, AID_SYSTEM);

   // VideoCodec
   chmod("/dev/Vcodec", 0660);
	chown("/dev/Vcodec", AID_MEDIA, AID_SYSTEM);

   // kpp
	chown("/sys/devices/platform/mtk-kpd/driver/kpd_call_state", AID_SYSTEM, AID_SYSTEM);

   // Charging Battery
	chown("/sys/devices/platform/battery/Charging_CallState", AID_SYSTEM, AID_SYSTEM);

	chown("/data/misc/acdapi/calib.dat", AID_SYSTEM, AID_SYSTEM);
	chown("/data/misc/acdapi/param.dat", AID_SYSTEM, AID_SYSTEM);
	chown("/data/misc/acdapi/sensors.dat", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/input/input5/enable", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/input/input5/delay", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/input/input5/wake", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/input/input3/enable", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/input/input3/wake", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/input/input4/offsets", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/class/input/input4/dynamic_matrix", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/devices/platform/msensor/driver/cpsopmode", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/devices/platform/msensor/driver/cpsreptxy", AID_SYSTEM, AID_SYSTEM);
	chown("/sys/devices/platform/msensor/driver/cpsreptz", AID_SYSTEM, AID_SYSTEM);

	// SPM SODI Enable
   chmod("/proc/mcdi/sodi_en", 0660);
	chown("/proc/mcdi/sodi_en", AID_SYSTEM, AID_SYSTEM);

   chmod("/dev/graphics/fb0", 0660);
	chown("/dev/graphics/fb0", AID_SYSTEM, AID_GRAPHICS);
   chmod("/dev/mtkfb_vsync", 0660);
	chown("/dev/mtkfb_vsync", AID_SYSTEM, AID_GRAPHICS);
   chmod("/dev/sw_sync", 0660);
 	chown("/dev/sw_sync", AID_SYSTEM, AID_GRAPHICS);

	// /dev/ttyMT1 for ext_mdinit
	chmod("/dev/ttyMT1", 0660);
	chown("/dev/ttyMT1", AID_SYSTEM, AID_SYSTEM);

   // pmu
	chown("/sys/devices/platform/mt-pmic/pmic_access", AID_ROOT, AID_RADIO);
   chmod("/sys/devices/platform/mt-pmic/pmic_access", 0664);

	// EM baseband file
	chown("/system/bin/baseband_regdump", AID_ROOT, AID_DIAG);
   chmod("/system/bin/baseband_regdump", 4750);
   //  EM CPU Speed Stress
	chown("/proc/cpu_ss/cpu_ss_debug_mode", AID_ROOT, AID_RADIO);
	chown("/proc/cpu_ss/cpu_ss_mode", AID_ROOT, AID_RADIO);
	chown("/proc/cpu_ss/cpu_ss_period", AID_ROOT, AID_RADIO);
	chown("/proc/cpu_ss/cpu_ss_period_mode", AID_ROOT, AID_RADIO);
   chmod("/proc/cpu_ss/cpu_ss_debug_mode", 0664);
   chmod("/proc/cpu_ss/cpu_ss_mode", 0664);
   chmod("/proc/cpu_ss/cpu_ss_period", 0664);
   chmod("/proc/cpu_ss/cpu_ss_period_mode", 0664);
   // EM eint
	chown("/sys/bus/platform/drivers/eint/current_eint", AID_ROOT, AID_DIAG);

	// MJC
	chmod("/dev/MJC", 0660);
	chown("/dev/MJC", AID_SYSTEM, AID_MEDIA);

   // autok
	chown("/sys/autok/debug", AID_ROOT, AID_SYSTEM);
	chown("/sys/autok/param_count", AID_ROOT, AID_SYSTEM);
	chown("/sys/autok/ready", AID_ROOT, AID_SYSTEM);
	chown("/sys/autok/ss_corner", AID_ROOT, AID_SYSTEM);
	chown("/sys/autok/suggest_vol", AID_ROOT, AID_SYSTEM);
	chown("/sys/autok/stage1/3/DONE", AID_ROOT, AID_SYSTEM);
	chown("/sys/autok/stage1/3/PARAMS", AID_ROOT, AID_SYSTEM);
	chown("/sys/autok/stage1/3/VOLTAGE", AID_ROOT, AID_SYSTEM);
	chown("/sys/autok/stage1/3/LOG", AID_ROOT, AID_SYSTEM);
	chown("/sys/autok/stage2/3", AID_ROOT, AID_SYSTEM);

	ALOGI("exiting");

	return 0;
}
