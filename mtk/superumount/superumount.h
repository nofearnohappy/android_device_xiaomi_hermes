int umount_dev(const char *srcdev, int src_major, int src_minor);
int destroyDM(const char *name);
static void usage(void);
void super_umount(char *src_dir);
int dmcreate(const char *name, const char *loopFile, const char *key,
                      unsigned int numSectors, char *ubuffer, size_t len, int dmtype);



