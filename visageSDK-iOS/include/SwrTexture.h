#ifndef SWR_TEXTURE_H
#define SWR_TEXTURE_H

// our texture class
struct SwrTexture {
	unsigned char *surface;

	unsigned w_log2, h_log2;
	unsigned w_minus1, h_minus1;

	// create a texture from an SDL_Surface
	SwrTexture(unsigned char *s, int w, int h);

	~SwrTexture();

	// returns log2 of a number which is a power of two
	unsigned log2_of_pot(unsigned v) const;

	// samples the texture using the given texture coordinate.
	// the integer texture coordinate is given in the upper 16 bits of the x and y variables.
	// it is NOT in the range [0.0, 1.0] but rather in the range of [0, width - 1] or [0, height - 1].
	// texture coordinates outside this range are wrapped.
	unsigned char sample_nearest(int x, int y) const;

};

#endif