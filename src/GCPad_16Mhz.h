#ifndef GCPAD_H_
#define GCPAD_H_

void GCPad_send(byte *cmd, byte length);
void GCPad_recv(byte *buffer, byte bits);
void GCPad_init();
byte *GCPad_read();
byte *N64Pad_read();

#endif /* GCPAD_H_ */
