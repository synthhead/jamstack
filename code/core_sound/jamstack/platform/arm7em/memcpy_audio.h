#ifndef memcpy_audio_h_
#define memcpy_audio_h_

#ifdef __cplusplus
extern "C"
{
#endif

  void memcpy_tointerleaveLR(int16_t* dst, const int16_t* srcL, const int16_t* srcR);
  void memcpy_tointerleaveL(int16_t* dst, const int16_t* srcL);
  void memcpy_tointerleaveR(int16_t* dst, const int16_t* srcR);
  void memcpy_tointerleaveQuad(int16_t* dst, const int16_t* src1, const int16_t* src2, const int16_t* src3, const int16_t* src4);

#ifdef __cplusplus
}
#endif

#endif
