#include "jpeg_gray_scale.h"

void escala_grises_jpeg (FILE *imagen_ent, FILE *imagen_salid) {

        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;

        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);
        jpeg_stdio_src(&cinfo, imagen_ent);
        jpeg_read_header(&cinfo, TRUE);

        cinfo.out_color_space = JCS_GRAYSCALE;
        jpeg_start_decompress(&cinfo);

        JSAMPROW row_buffer[1];
        row_buffer[0] = (JSAMPROW)malloc(cinfo.output_width * cinfo.output_components);

        while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, row_buffer, 1);
        fwrite(row_buffer[0], 1, cinfo.output_width, imagen_salid);
        }

        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
}
