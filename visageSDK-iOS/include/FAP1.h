
#ifndef __FAP1_h__
#define __FAP1_h__

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

namespace VisageSDK
{

/**
* MPEG-4 FAP1 (viseme).
* The storage class for the MPEG-4 viseme (FAP1). It contains
* the full definition of FAP1 (viseme) according to MPEG-4 specification.
* A summary of the MPEG-4 FAP specification can be found in the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>. 
* 
* This class specifies the current mouth shape as a blend between two visemes: viseme1 and viseme2. Each of them is an integer value between 0 and 14 that 
* specifies the viseme acording to the MPEG-4 standard. The detailed description of the 14 visemes, including illustration of mouth shapes, is
* provided in the Annex 2 (page 37) of the <a href="MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>. Briefly, the 14
* visemes are:
*
* <table>
* <tr><td width="100"><b>VISEME NUMBER</b></td><td width="200"><b>VISEME</b></td></tr>
* <tr><td>0</td><td>silence</td></tr>
* <tr><td>1</td><td>PP (p, b, m) as in put, bed, mill</td></tr>
* <tr><td>2</td><td>FF (f, v) as in far, voice</td></tr>
* <tr><td>3</td><td>TH (T, D) as in think, that</td></tr>
* <tr><td>4</td><td>DD (t, d) as in tip, doll</td></tr>
* <tr><td>5</td><td>kk (k, g) as in call, gas</td></tr>
* <tr><td>6</td><td>CH (tS, dZ, S) as in chair, join, she</td></tr>
* <tr><td>7</td><td>SS (s, z) as in sir, zeal</td></tr>
* <tr><td>8</td><td>nn (n, l) as in not, lot</td></tr>
* <tr><td>9</td><td>RR (r) as in red</td></tr>
* <tr><td>10</td><td>AA (A:) as in car</td></tr>
* <tr><td>11</td><td>E (e) as in bed</td></tr>
* <tr><td>12</td><td>ih (I) as in tip</td></tr>
* <tr><td>13</td><td>oh (O) as in top</td></tr>
* <tr><td>14</td><td>ou (U) as in book</td></tr>
* </table>
*
* The blend factor is an integer value between 0 and 63 that specifies the blending between viseme1 and viseme2 that results in the final mouth shape.
* When blend is 0, the mouth shape is viseme2; when blend is 63 the mouth shape is viseme1. Other values of blend result in linear interpolation
* between mouth shapes for viseme1 and viseme2, so for example blend value 32 results in a mouth shape that is an equal mix of both visemes.
* The low-level algorithm for blending the mouth shapes may vary from one 3D engine to another, but most commonly it is implemented by morphing.
*
* This class also provides functions for merging visemes. 
*
* The value of the visemes can be set directly by accessing the public attributes of this class, or through convenience function
* FBAPs::setViseme(). 
*
* Implemented in visagefbadecoder.lib
*/
class VISAGE_DECLSPEC FAP1  
{
public:
	/**
	* Constructor.
	* Constructs a FAP1 with no active viseme (all values set to 0).
	*/
	FAP1();

	/**
	* Constructor.
	* Constructs a FAP1 by copying a given FAP1.
	* @param f the FAP1 to be copied.
	*/
	FAP1(FAP1 *f);

	/**
	* Set the FAP1.
	* Set the FAP1 by copying a given FAP1 
	* @param f the FAP1 to be copied.
	*/
	void set(const FAP1 *f);

	/**
	* Destructor.
	*/
	virtual ~FAP1();

	/**
	* Merge with another FAP1.
	* Merge FAP1 with the given FAP1.
	* The two FAP1 instances are merged as follows:
	* if this FAP1 has a viseme
	* already set to non-zero value, its value is kept and the merged FAP1 has no influence on it. If this FAP1 has
	* no active viseme, the values from the merged FAP1 are taken.
	*
	* @param f the FAP1 to be merged with this FAP1.
	*/
	void merge(FAP1 *f);

	/**
	* Test if the FAP1 is void.
	* Test whether the FAP1 is void, i.e. if there is no viseme (both viseme1 and viseme2 are 0.
	* @return 1 if it is void (no viseme), 0 otherwise.
	*/
	int isVoid();

	/**
	* Test if this FAP1 is equal to another FAP1.
	* Test whether the this FAP1 is equal to another FAP1.
	* @param f the FAP1 to be compared with this FAP1.
	* @return true if equal, false otherwise.
	*/
	bool isEqual(FAP1 *f);

	/**
	* Viseme 1.
	* Viseme 1, according to MPEG-4 FBA specification. One of the two viseme that, together with the blend factor, specify the 
	* mouth shape. It takes an integer value between 0 and 14 that specifies the viseme according to the MPEG-4 specification.
	* The detailed description of the 14 visemes, including illustration of mouth shapes, is
	* provided in the Annex 2 (page 37) of the <a href="MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>. Briefly, the 14
	* visemes are:
	*
	* <table>
	* <tr><td width="100"><b>VISEME NUMBER</b></td><td width="200"><b>VISEME</b></td></tr>
	* <tr><td>0</td><td>silence</td></tr>
	* <tr><td>1</td><td>PP (p, b, m) as in put, bed, mill</td></tr>
	* <tr><td>2</td><td>FF (f, v) as in far, voice</td></tr>
	* <tr><td>3</td><td>TH (T, D) as in think, that</td></tr>
	* <tr><td>4</td><td>DD (t, d) as in tip, doll</td></tr>
	* <tr><td>5</td><td>kk (k, g) as in call, gas</td></tr>
	* <tr><td>6</td><td>CH (tS, dZ, S) as in chair, join, she</td></tr>
	* <tr><td>7</td><td>SS (s, z) as in sir, zeal</td></tr>
	* <tr><td>8</td><td>nn (n, l) as in not, lot</td></tr>
	* <tr><td>9</td><td>RR (r) as in red</td></tr>
	* <tr><td>10</td><td>AA (A:) as in car</td></tr>
	* <tr><td>11</td><td>E (e) as in bed</td></tr>
	* <tr><td>12</td><td>ih (I) as in tip</td></tr>
	* <tr><td>13</td><td>oh (O) as in top</td></tr>
	* <tr><td>14</td><td>ou (U) as in book</td></tr>
	* </table>
	*/
	int viseme1;

	/**
	* Viseme 2.
	* Viseme 2, according to MPEG-4 FBA specification. One of the two viseme that, together with the blend factor, specify the 
	* mouth shape. It takes an integer value between 0 and 14 that specifies the viseme according to the MPEG-4 specification.
	* The detailed description of the 14 visemes, including illustration of mouth shapes, is
	* provided in the Annex 2 (page 37) of the <a href="MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>. Briefly, the 14
	* visemes are:
	*
	* <table>
	* <tr><td width="100"><b>VISEME NUMBER</b></td><td width="200"><b>VISEME</b></td></tr>
	* <tr><td>0</td><td>silence</td></tr>
	* <tr><td>1</td><td>PP (p, b, m) as in put, bed, mill</td></tr>
	* <tr><td>2</td><td>FF (f, v) as in far, voice</td></tr>
	* <tr><td>3</td><td>TH (T, D) as in think, that</td></tr>
	* <tr><td>4</td><td>DD (t, d) as in tip, doll</td></tr>
	* <tr><td>5</td><td>kk (k, g) as in call, gas</td></tr>
	* <tr><td>6</td><td>CH (tS, dZ, S) as in chair, join, she</td></tr>
	* <tr><td>7</td><td>SS (s, z) as in sir, zeal</td></tr>
	* <tr><td>8</td><td>nn (n, l) as in not, lot</td></tr>
	* <tr><td>9</td><td>RR (r) as in red</td></tr>
	* <tr><td>10</td><td>AA (A:) as in car</td></tr>
	* <tr><td>11</td><td>E (e) as in bed</td></tr>
	* <tr><td>12</td><td>ih (I) as in tip</td></tr>
	* <tr><td>13</td><td>oh (O) as in top</td></tr>
	* <tr><td>14</td><td>ou (U) as in book</td></tr>
	* </table>
	*/
	int viseme2;

	/**
	* viseme_blend.
	* viseme_blend, according to MPEG-4 FBA specification.
	* It is an integer value between 0 and 63 that specifies the blending between viseme1 and viseme2 that results in the final mouth shape.
	* When blend is 0, the mouth shape is viseme2; when blend is 63 the mouth shape is viseme1. Other values of blend result in linear interpolation
	* between mouth shapes for viseme1 and viseme2, so for example blend value 32 results in a mouth shape that is an equal mix of both visemes.
	*/
	int blend;

	/**
	* def_bit.
	* def_bit, according to MPEG-4 FBA specification. This has no influence on animation and is used only for encoding purposes.
	*/
	int defBit;

	/**
	* Reset.
	* Reset the neutral position, i.e. all parameters are set to 0.
	*/
	void reset();
};

}

#endif // __FAP1_h__
