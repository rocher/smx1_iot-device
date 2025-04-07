#ifndef NTP_H
#define NTP_H

extern const bool  LOG_ERRORS;
extern const char *NTP_SERVER;
extern const int   NTP_TIMEOUT;
extern const char  TZ_DST[];

void ntp_setup();
void ntp_update();

#endif // NTP_H
