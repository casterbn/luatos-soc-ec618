/*
 * Copyright (c) 2022 OpenLuat & AirM2M
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "common_api.h"
#include "FreeRTOS.h"
#include "luat_base.h"
#include "luat_fs.h"
#include "osasys.h"
#include "lfs_port.h"

#include "mem_map.h"


#ifdef LUAT_USE_FS_VFS

FILE* luat_vfs_ec618_fopen(void* userdata, const char *filename, const char *mode) {
    FILE* f = (FILE*)OsaFopen(filename, mode);
    //DBG("luat_fs_fopen %s %s %p", filename, mode, f);
    return f;
}

int luat_vfs_ec618_getc(void* userdata, FILE* stream) {
    char buff[1];
    buff[0] = 0;
    OsaFread(buff, 1, 1, stream);
    return buff[0];
}

int luat_vfs_ec618_fseek(void* userdata, FILE* stream, long int offset, int origin) {
    //DBG("luat_fs_fseek fd=%p offset=%ld ori=%ld", stream, offset, origin);
    return OsaFseek(stream, offset, origin);
}

int luat_vfs_ec618_ftell(void* userdata, FILE* stream) {
    //DBG("luat_fs_ftell fd=%ld", stream);
    //return OsaFseek(stream, 0, SEEK_CUR);
    return LFS_fileTell((lfs_file_t *)stream);
}

int luat_vfs_ec618_fclose(void* userdata, FILE* stream) {
    //DBG("luat_fs_fclose fd=%p", stream);
    return OsaFclose(stream);
}
int luat_vfs_ec618_feof(void* userdata, FILE* stream) {
    //int ret = OsaFsize(stream) == OsaFseek(stream, 0, SEEK_CUR);
    //DBG("luat_fs_feof fd=%ld size=%ld pos=%ld ret=%ld", stream, OsaFsize(stream), OsaFseek(stream, 0, SEEK_CUR), ret);
    //return ret;
    return LFS_fileTell((lfs_file_t *)stream) == LFS_fileSize((lfs_file_t *)stream) ? 1 : 0;
}
int luat_vfs_ec618_ferror(void* userdata, FILE *stream) {
    return 0;
}
size_t luat_vfs_ec618_fread(void* userdata, void *ptr, size_t size, size_t nmemb, FILE *stream) {
    char *data = (char*)ptr;
    size_t t = OsaFread(data, size, nmemb, stream);
    //DBG("luat_fs_fread fd=%p size=%ld nmemb=%ld ret=%ld", stream, size, nmemb, t);
    //DBG("luat_fs_fread data[0-7] %p %X %X %X %X %X %X %X %X", data, *(data), *(data+1), *(data+2), *(data+3), *(data+4), *(data+5), *(data+6), *(data+7));
    return t;
}
size_t luat_vfs_ec618_fwrite(void* userdata, const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    //DBG("luat_fs_fwrite fd=%p size=%ld nmemb=%ld", stream, size, nmemb);
    return OsaFwrite((void*)ptr, size, nmemb, stream);
}
int luat_vfs_ec618_remove(void* userdata, const char *filename) {
    return OsaFremove(filename);
}
int luat_vfs_ec618_rename(void* userdata, const char *old_filename, const char *new_filename) {
    return -1;
}
int luat_vfs_ec618_fexist(void* userdata, const char *filename) {
    FILE* fd = luat_vfs_ec618_fopen(userdata, filename, "rb");
    if (fd) {
        luat_vfs_ec618_fclose(userdata, fd);
        return 1;
    }
    return 0;
}

size_t luat_vfs_ec618_fsize(void* userdata, const char *filename) {
    FILE* fd = OsaFopen(filename, "rb");
    if (fd) {
        size_t sz = LFS_fileSize((lfs_file_t *)fd);
        OsaFclose(fd);
        return sz;
    }
    return 0;
}

int luat_vfs_ec618_mkfs(void* userdata, luat_fs_conf_t *conf) {
    DBG("not support yet : mkfs");
    return -1;
}
int luat_vfs_ec618_mount(void** userdata, luat_fs_conf_t *conf) {
    //DBG("not support yet : mount");
    return 0;
}
int luat_vfs_ec618_umount(void* userdata, luat_fs_conf_t *conf) {
    //DBG("not support yet : umount");
    return 0;
}

int luat_vfs_ec618_mkdir(void* userdata, char const* _DirName) {
    DBG("not support yet : mkdir");
    return -1;
}
int luat_vfs_ec618_rmdir(void* userdata, char const* _DirName) {
    return LFS_remove(_DirName);
}

int luat_vfs_ec618_info(void* userdata, const char* path, luat_fs_info_t *conf) {
    if (strcmp("/", path)) {
        DBG("not support path %s", path);
        return -2;
    }
    lfs_status_t status;
    if (!LFS_statfs(&status)) {
        conf->total_block = status.total_block;
        conf->block_used = status.block_used / status.block_size + 1;
        conf->block_size = status.block_size;
        conf->type = 1; // 片上FLASH
        memcpy(conf->filesystem, "lfs", 3);
        conf->filesystem[4] = 0;
        return 0;
    }
    else {
        DBG("LFS_Statfs return != 0");
    }
    return -1;
}


#define T(name) .name = luat_vfs_ec618_##name
const struct luat_vfs_filesystem vfs_fs_ec618 = {
    .name = "ec618",
    .opts = {
        T(mkfs),
        T(mount),
        T(umount),
        T(mkdir),
        T(rmdir),
        T(remove),
        T(rename),
        T(fsize),
        T(fexist),
        T(info)
    },
    .fopts = {
        T(fopen),
        T(getc),
        T(fseek),
        T(ftell),
        T(fclose),
        T(feof),
        T(ferror),
        T(fread),
        T(fwrite)
    }
};

#ifdef __LUATOS__
extern const struct luat_vfs_filesystem vfs_fs_luadb;
#endif

int luat_fs_init(void) {

    luat_vfs_reg(&vfs_fs_ec618);
#ifdef __LUATOS__
	luat_vfs_reg(&vfs_fs_luadb);
#endif

	static const luat_fs_conf_t conf = {
		.busname = "",
		.type = "ec618",
		.filesystem = "ec618",
		.mount_point = ""
	};
	luat_fs_mount(&conf);

#ifdef __LUATOS__
    // 以下为临时配置, 从APP区的末端,切出128k作为临时脚本区
    #define LUADB_ADDR ((uint32_t)(FLASH_FOTA_REGION_START - 128 * 1024) + 0x800000)
    //DBG("luadb tmp addr %p", LUADB_ADDR);
	static const luat_fs_conf_t conf2 = {
		.busname = (char*)(const char*)LUADB_ADDR,
		.type = "luadb",
		.filesystem = "luadb",
		.mount_point = "/luadb/",
	};
	luat_fs_mount(&conf2);
#ifdef LUAT_USE_LVGL
	luat_lv_fs_init();
	// lv_bmp_init();
	// lv_png_init();
	lv_split_jpeg_init();
#endif
#endif
	return 0;
}

#else

static inline const char* pwrap(const char* path) {
    if (path == NULL)
        return NULL;
    if (path[0] == '/')
        return path + 1;
    return path;
} 

FILE* luat_fs_fopen(const char *filename, const char *mode) {
    FILE* f = (FILE*)OsaFopen(pwrap(filename), mode);
    //DBG("luat_fs_fopen %s %s %p", filename, mode, f);
    return f;
}

int luat_fs_getc(FILE* stream) {
    char buff[1];
    buff[0] = 0;
    OsaFread(buff, 1, 1, stream);
    return buff[0];
}

int luat_fs_fseek(FILE* stream, long int offset, int origin) {
    //DBG("luat_fs_fseek fd=%p offset=%ld ori=%ld", stream, offset, origin);
    return OsaFseek(stream, offset, origin);
}

int luat_fs_ftell(FILE* stream) {
    //DBG("luat_fs_ftell fd=%ld", stream);
    //return OsaFseek(stream, 0, SEEK_CUR);
    return LFS_fileTell((lfs_file_t *)stream);
}

int luat_fs_fclose(FILE* stream) {
    //DBG("luat_fs_fclose fd=%p", stream);
    return OsaFclose(stream);
}
int luat_fs_feof(FILE* stream) {
    //int ret = OsaFsize(stream) == OsaFseek(stream, 0, SEEK_CUR);
    //DBG("luat_fs_feof fd=%ld size=%ld pos=%ld ret=%ld", stream, OsaFsize(stream), OsaFseek(stream, 0, SEEK_CUR), ret);
    //return ret;
    return LFS_fileTell((lfs_file_t *)stream) == LFS_fileSize((lfs_file_t *)stream) ? 1 : 0;
}
int luat_fs_ferror(FILE *stream) {
    return 0;
}
size_t luat_fs_fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    char *data = (char*)ptr;
    size_t t = OsaFread(data, size, nmemb, stream);
    //DBG("luat_fs_fread fd=%p size=%ld nmemb=%ld ret=%ld", stream, size, nmemb, t);
    //DBG("luat_fs_fread data[0-7] %p %X %X %X %X %X %X %X %X", data, *(data), *(data+1), *(data+2), *(data+3), *(data+4), *(data+5), *(data+6), *(data+7));
    return t;
}
size_t luat_fs_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    //DBG("luat_fs_fwrite fd=%p size=%ld nmemb=%ld", stream, size, nmemb);
    return OsaFwrite((void*)ptr, size, nmemb, stream);
}
int luat_fs_remove(const char *filename) {
    return OsaFremove(pwrap(filename));
}
int luat_fs_rename(const char *old_filename, const char *new_filename) {
    return LFS_rename(pwrap(old_filename), pwrap(new_filename));
}
int luat_fs_fexist(const char *filename) {
    FILE* fd = luat_fs_fopen(pwrap(filename), "rb");
    if (fd) {
        luat_fs_fclose(fd);
        return 1;
    }
    return 0;
}

size_t luat_fs_fsize(const char *filename) {
    FILE* fd = OsaFopen(pwrap(filename), "rb");
    if (fd) {
        size_t sz = LFS_fileSize((lfs_file_t *)fd);
        OsaFclose(fd);
        return sz;
    }
    return 0;
}

int luat_fs_ftruncate(FILE* fp, size_t len) {
    return LFS_fileTruncate(fp, len);
}

int luat_fs_truncate(const char* filename, size_t len) {
    lfs_file_t file = {0};
    int ret = LFS_fileOpen(&file, pwrap(filename), LFS_O_RDWR);
    if (ret == 0) {
        LFS_fileTruncate(&file, len);
        LFS_fileClose(&file);
        return len;
    }
    return 0;
}

int luat_fs_mkfs(luat_fs_conf_t *conf) {
    DBG("not support yet : mkfs");
    return -1;
}
int luat_fs_mount(luat_fs_conf_t *conf) {
    //DBG("not support yet : mount");
    return 0;
}
int luat_fs_umount(luat_fs_conf_t *conf) {
    //DBG("not support yet : umount");
    return 0;
}

int luat_fs_mkdir(char const* dir) {
    DBG("not support yet : mkdir");
    return -1;
}
int luat_fs_rmdir(char const* dir) {
    if (dir == NULL || (strlen(dir) == 1 && dir[0] == '/')) {
        return 0;
    }
    return LFS_remove(pwrap(dir));
}

int luat_fs_info(const char* path, luat_fs_info_t *conf) {
    if (strcmp("/", path)) {
        DBG("not support path %s", path);
        return -2;
    }
    lfs_status_t status;
    if (!LFS_statfs(&status)) {
        conf->total_block = status.total_block;
        conf->block_used = status.block_used / status.block_size + 1;
        conf->block_size = status.block_size;
        conf->type = 1; // 片上FLASH
        memcpy(conf->filesystem, "lfs", 3);
        conf->filesystem[4] = 0;
        return 0;
    }
    else {
        DBG("LFS_Statfs return != 0");
    }
    return -1;
}

int luat_fs_init(void) {
    return 0;
}

#endif

