#include <stdio.h>
#include <stdint.h>

#include <rom5e.h>	// tiles
#include <rom5f.h>	// sprites

void draw_slice(uint8_t *d, int dim, uint8_t *c, int offset, int x, int y) {
	offset *= 32;
	for (int n = 7; n >= 0; n--)
		for (int m = 3; m >= 0; m--) {          // 4 rows
			unsigned px = x+n, py = y+m;
			d[dim*px + py] = c[offset];
			offset++;
		}
}

int main(int argc, char *argv[]) {

	printf("static const unsigned char tiles[] = {");
	for (int t = 0; t < 256; t++) {
		uint8_t c[64], d[64];
		for (int i = 0; i < 16; i++) {
                	uint8_t b = tiles[t*16 + i];
                	c[i*4  ] =  (b       & 0x01) | ((b >> 3) & 0x02);
                	c[i*4+1] = ((b >> 1) & 0x01) | ((b >> 4) & 0x02);
                	c[i*4+2] = ((b >> 2) & 0x01) | ((b >> 5) & 0x02);
                	c[i*4+3] = ((b >> 3) & 0x01) | ((b >> 6) & 0x02);
		}
		draw_slice(d, 8, c, 0, 0, 4);
		draw_slice(d, 8, c, 1, 0, 0);

		for (int i = 0; i < sizeof(d); i++) {
			if ((i % 8) == 0) printf("\n\t");
			printf("0x%02x, ", d[i]);
		}
	}
	printf("\n};\n");

	printf("static const unsigned char sprites[] = {");
	for (int s = 0; s < 64; s++) {
		uint8_t c[256], d[256];
		for (int i = 0; i < 64; i++) {
                	uint8_t b = sprites[s*64 + i];
                	c[i*4  ] =  (b       & 0x01) | ((b >> 3) & 0x02);
                	c[i*4+1] = ((b >> 1) & 0x01) | ((b >> 4) & 0x02);
                	c[i*4+2] = ((b >> 2) & 0x01) | ((b >> 5) & 0x02);
                	c[i*4+3] = ((b >> 3) & 0x01) | ((b >> 6) & 0x02);
		}
		draw_slice(d, 16, c, 0, 8, 12);
		draw_slice(d, 16, c, 1, 8, 0);
		draw_slice(d, 16, c, 2, 8, 4);
		draw_slice(d, 16, c, 3, 8, 8);
		draw_slice(d, 16, c, 4, 0, 12);
		draw_slice(d, 16, c, 5, 0, 0);
		draw_slice(d, 16, c, 6, 0, 4);
		draw_slice(d, 16, c, 7, 0, 8);

		for (int i = 0; i < sizeof(d); i++) {
			if ((i % 8) == 0) printf("\n\t");
			printf("0x%02x, ", d[i]);
		}
	}
	printf("\n};\n");
}
