#ifdef _RA0
#ifdef _CNPUA0
#define ENABLE_RA0_PULLUP() _CNPUA0 = 1
#define DISABLE_RA0_PULLUP() _CNPUA0 = 0
#endif
#ifdef _CNPDA0
#define ENABLE_RA0_PULLDOWN() _CNPDA0 = 1
#define DISABLE_RA0_PULLDOWN() _CNPDA0 = 0
#endif
#ifdef _ANSA0
#define ENABLE_RA0_ANALOG() _ANSA0 = 1
#define DISABLE_RA0_ANALOG() _ANSA0 = 0
#endif
#ifdef _CNIEA0
#define ENABLE_RA0_CN_INTERRUPT() _CNIEA0 = 1
#define DISABLE_RA0_CN_INTERRUPT() _CNIEA0 = 0
#endif
static inline void CONFIG_RA0_AS_DIG_OUTPUT() {
#ifdef _CNPUA0
  _CNPUA0 = 0; //disable pullup
#endif
#ifdef _CNPDA0
  _CNPDA0 = 0; //disable pulldown
#endif
#ifdef _ODCA0
  _ODCA0 = 0;
#endif
#ifdef _TRISA0
  _TRISA0 = 0;
#endif
#ifdef _ANSA0
  _ANSA0 = 0;  //disable analog
#endif
}
#ifdef _ODCA0
#define ENABLE_RA0_OPENDRAIN() _ODCA0 = 1
#define DISABLE_RA0_OPENDRAIN() _ODCA0 = 0
#define CONFIG_RA0_AS_DIG_OD_OUTPUT()  {CONFIG_RA0_AS_DIG_OUTPUT();ENABLE_RA0_OPENDRAIN();}
#endif
static inline void CONFIG_RA0_AS_DIG_INPUT() {
#ifdef _CNPUA0
  _CNPUA0 = 0; //disable pullup
#endif
#ifdef _CNPDA0
  _CNPDA0 = 0; //disable pulldown
#endif
#ifdef _TRISA0
  _TRISA0 = 1;
#endif
#ifdef _ANSA0
  _ANSA0 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA0
#ifdef _RA1
#ifdef _CNPUA1
#define ENABLE_RA1_PULLUP() _CNPUA1 = 1
#define DISABLE_RA1_PULLUP() _CNPUA1 = 0
#endif
#ifdef _CNPDA1
#define ENABLE_RA1_PULLDOWN() _CNPDA1 = 1
#define DISABLE_RA1_PULLDOWN() _CNPDA1 = 0
#endif
#ifdef _ANSA1
#define ENABLE_RA1_ANALOG() _ANSA1 = 1
#define DISABLE_RA1_ANALOG() _ANSA1 = 0
#endif
#ifdef _CNIEA1
#define ENABLE_RA1_CN_INTERRUPT() _CNIEA1 = 1
#define DISABLE_RA1_CN_INTERRUPT() _CNIEA1 = 0
#endif
static inline void CONFIG_RA1_AS_DIG_OUTPUT() {
#ifdef _CNPUA1
  _CNPUA1 = 0; //disable pullup
#endif
#ifdef _CNPDA1
  _CNPDA1 = 0; //disable pulldown
#endif
#ifdef _ODCA1
  _ODCA1 = 0;
#endif
#ifdef _TRISA1
  _TRISA1 = 0;
#endif
#ifdef _ANSA1
  _ANSA1 = 0;  //disable analog
#endif
}
#ifdef _ODCA1
#define ENABLE_RA1_OPENDRAIN() _ODCA1 = 1
#define DISABLE_RA1_OPENDRAIN() _ODCA1 = 0
#define CONFIG_RA1_AS_DIG_OD_OUTPUT()  {CONFIG_RA1_AS_DIG_OUTPUT();ENABLE_RA1_OPENDRAIN();}
#endif
static inline void CONFIG_RA1_AS_DIG_INPUT() {
#ifdef _CNPUA1
  _CNPUA1 = 0; //disable pullup
#endif
#ifdef _CNPDA1
  _CNPDA1 = 0; //disable pulldown
#endif
#ifdef _TRISA1
  _TRISA1 = 1;
#endif
#ifdef _ANSA1
  _ANSA1 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA1
#ifdef _RA2
#ifdef _CNPUA2
#define ENABLE_RA2_PULLUP() _CNPUA2 = 1
#define DISABLE_RA2_PULLUP() _CNPUA2 = 0
#endif
#ifdef _CNPDA2
#define ENABLE_RA2_PULLDOWN() _CNPDA2 = 1
#define DISABLE_RA2_PULLDOWN() _CNPDA2 = 0
#endif
#ifdef _ANSA2
#define ENABLE_RA2_ANALOG() _ANSA2 = 1
#define DISABLE_RA2_ANALOG() _ANSA2 = 0
#endif
#ifdef _CNIEA2
#define ENABLE_RA2_CN_INTERRUPT() _CNIEA2 = 1
#define DISABLE_RA2_CN_INTERRUPT() _CNIEA2 = 0
#endif
static inline void CONFIG_RA2_AS_DIG_OUTPUT() {
#ifdef _CNPUA2
  _CNPUA2 = 0; //disable pullup
#endif
#ifdef _CNPDA2
  _CNPDA2 = 0; //disable pulldown
#endif
#ifdef _ODCA2
  _ODCA2 = 0;
#endif
#ifdef _TRISA2
  _TRISA2 = 0;
#endif
#ifdef _ANSA2
  _ANSA2 = 0;  //disable analog
#endif
}
#ifdef _ODCA2
#define ENABLE_RA2_OPENDRAIN() _ODCA2 = 1
#define DISABLE_RA2_OPENDRAIN() _ODCA2 = 0
#define CONFIG_RA2_AS_DIG_OD_OUTPUT()  {CONFIG_RA2_AS_DIG_OUTPUT();ENABLE_RA2_OPENDRAIN();}
#endif
static inline void CONFIG_RA2_AS_DIG_INPUT() {
#ifdef _CNPUA2
  _CNPUA2 = 0; //disable pullup
#endif
#ifdef _CNPDA2
  _CNPDA2 = 0; //disable pulldown
#endif
#ifdef _TRISA2
  _TRISA2 = 1;
#endif
#ifdef _ANSA2
  _ANSA2 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA2
#ifdef _RA3
#ifdef _CNPUA3
#define ENABLE_RA3_PULLUP() _CNPUA3 = 1
#define DISABLE_RA3_PULLUP() _CNPUA3 = 0
#endif
#ifdef _CNPDA3
#define ENABLE_RA3_PULLDOWN() _CNPDA3 = 1
#define DISABLE_RA3_PULLDOWN() _CNPDA3 = 0
#endif
#ifdef _ANSA3
#define ENABLE_RA3_ANALOG() _ANSA3 = 1
#define DISABLE_RA3_ANALOG() _ANSA3 = 0
#endif
#ifdef _CNIEA3
#define ENABLE_RA3_CN_INTERRUPT() _CNIEA3 = 1
#define DISABLE_RA3_CN_INTERRUPT() _CNIEA3 = 0
#endif
static inline void CONFIG_RA3_AS_DIG_OUTPUT() {
#ifdef _CNPUA3
  _CNPUA3 = 0; //disable pullup
#endif
#ifdef _CNPDA3
  _CNPDA3 = 0; //disable pulldown
#endif
#ifdef _ODCA3
  _ODCA3 = 0;
#endif
#ifdef _TRISA3
  _TRISA3 = 0;
#endif
#ifdef _ANSA3
  _ANSA3 = 0;  //disable analog
#endif
}
#ifdef _ODCA3
#define ENABLE_RA3_OPENDRAIN() _ODCA3 = 1
#define DISABLE_RA3_OPENDRAIN() _ODCA3 = 0
#define CONFIG_RA3_AS_DIG_OD_OUTPUT()  {CONFIG_RA3_AS_DIG_OUTPUT();ENABLE_RA3_OPENDRAIN();}
#endif
static inline void CONFIG_RA3_AS_DIG_INPUT() {
#ifdef _CNPUA3
  _CNPUA3 = 0; //disable pullup
#endif
#ifdef _CNPDA3
  _CNPDA3 = 0; //disable pulldown
#endif
#ifdef _TRISA3
  _TRISA3 = 1;
#endif
#ifdef _ANSA3
  _ANSA3 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA3
#ifdef _RA4
#ifdef _CNPUA4
#define ENABLE_RA4_PULLUP() _CNPUA4 = 1
#define DISABLE_RA4_PULLUP() _CNPUA4 = 0
#endif
#ifdef _CNPDA4
#define ENABLE_RA4_PULLDOWN() _CNPDA4 = 1
#define DISABLE_RA4_PULLDOWN() _CNPDA4 = 0
#endif
#ifdef _ANSA4
#define ENABLE_RA4_ANALOG() _ANSA4 = 1
#define DISABLE_RA4_ANALOG() _ANSA4 = 0
#endif
#ifdef _CNIEA4
#define ENABLE_RA4_CN_INTERRUPT() _CNIEA4 = 1
#define DISABLE_RA4_CN_INTERRUPT() _CNIEA4 = 0
#endif
static inline void CONFIG_RA4_AS_DIG_OUTPUT() {
#ifdef _CNPUA4
  _CNPUA4 = 0; //disable pullup
#endif
#ifdef _CNPDA4
  _CNPDA4 = 0; //disable pulldown
#endif
#ifdef _ODCA4
  _ODCA4 = 0;
#endif
#ifdef _TRISA4
  _TRISA4 = 0;
#endif
#ifdef _ANSA4
  _ANSA4 = 0;  //disable analog
#endif
}
#ifdef _ODCA4
#define ENABLE_RA4_OPENDRAIN() _ODCA4 = 1
#define DISABLE_RA4_OPENDRAIN() _ODCA4 = 0
#define CONFIG_RA4_AS_DIG_OD_OUTPUT()  {CONFIG_RA4_AS_DIG_OUTPUT();ENABLE_RA4_OPENDRAIN();}
#endif
static inline void CONFIG_RA4_AS_DIG_INPUT() {
#ifdef _CNPUA4
  _CNPUA4 = 0; //disable pullup
#endif
#ifdef _CNPDA4
  _CNPDA4 = 0; //disable pulldown
#endif
#ifdef _TRISA4
  _TRISA4 = 1;
#endif
#ifdef _ANSA4
  _ANSA4 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA4
#ifdef _RA5
#ifdef _CNPUA5
#define ENABLE_RA5_PULLUP() _CNPUA5 = 1
#define DISABLE_RA5_PULLUP() _CNPUA5 = 0
#endif
#ifdef _CNPDA5
#define ENABLE_RA5_PULLDOWN() _CNPDA5 = 1
#define DISABLE_RA5_PULLDOWN() _CNPDA5 = 0
#endif
#ifdef _ANSA5
#define ENABLE_RA5_ANALOG() _ANSA5 = 1
#define DISABLE_RA5_ANALOG() _ANSA5 = 0
#endif
#ifdef _CNIEA5
#define ENABLE_RA5_CN_INTERRUPT() _CNIEA5 = 1
#define DISABLE_RA5_CN_INTERRUPT() _CNIEA5 = 0
#endif
static inline void CONFIG_RA5_AS_DIG_OUTPUT() {
#ifdef _CNPUA5
  _CNPUA5 = 0; //disable pullup
#endif
#ifdef _CNPDA5
  _CNPDA5 = 0; //disable pulldown
#endif
#ifdef _ODCA5
  _ODCA5 = 0;
#endif
#ifdef _TRISA5
  _TRISA5 = 0;
#endif
#ifdef _ANSA5
  _ANSA5 = 0;  //disable analog
#endif
}
#ifdef _ODCA5
#define ENABLE_RA5_OPENDRAIN() _ODCA5 = 1
#define DISABLE_RA5_OPENDRAIN() _ODCA5 = 0
#define CONFIG_RA5_AS_DIG_OD_OUTPUT()  {CONFIG_RA5_AS_DIG_OUTPUT();ENABLE_RA5_OPENDRAIN();}
#endif
static inline void CONFIG_RA5_AS_DIG_INPUT() {
#ifdef _CNPUA5
  _CNPUA5 = 0; //disable pullup
#endif
#ifdef _CNPDA5
  _CNPDA5 = 0; //disable pulldown
#endif
#ifdef _TRISA5
  _TRISA5 = 1;
#endif
#ifdef _ANSA5
  _ANSA5 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA5
#ifdef _RA6
#ifdef _CNPUA6
#define ENABLE_RA6_PULLUP() _CNPUA6 = 1
#define DISABLE_RA6_PULLUP() _CNPUA6 = 0
#endif
#ifdef _CNPDA6
#define ENABLE_RA6_PULLDOWN() _CNPDA6 = 1
#define DISABLE_RA6_PULLDOWN() _CNPDA6 = 0
#endif
#ifdef _ANSA6
#define ENABLE_RA6_ANALOG() _ANSA6 = 1
#define DISABLE_RA6_ANALOG() _ANSA6 = 0
#endif
#ifdef _CNIEA6
#define ENABLE_RA6_CN_INTERRUPT() _CNIEA6 = 1
#define DISABLE_RA6_CN_INTERRUPT() _CNIEA6 = 0
#endif
static inline void CONFIG_RA6_AS_DIG_OUTPUT() {
#ifdef _CNPUA6
  _CNPUA6 = 0; //disable pullup
#endif
#ifdef _CNPDA6
  _CNPDA6 = 0; //disable pulldown
#endif
#ifdef _ODCA6
  _ODCA6 = 0;
#endif
#ifdef _TRISA6
  _TRISA6 = 0;
#endif
#ifdef _ANSA6
  _ANSA6 = 0;  //disable analog
#endif
}
#ifdef _ODCA6
#define ENABLE_RA6_OPENDRAIN() _ODCA6 = 1
#define DISABLE_RA6_OPENDRAIN() _ODCA6 = 0
#define CONFIG_RA6_AS_DIG_OD_OUTPUT()  {CONFIG_RA6_AS_DIG_OUTPUT();ENABLE_RA6_OPENDRAIN();}
#endif
static inline void CONFIG_RA6_AS_DIG_INPUT() {
#ifdef _CNPUA6
  _CNPUA6 = 0; //disable pullup
#endif
#ifdef _CNPDA6
  _CNPDA6 = 0; //disable pulldown
#endif
#ifdef _TRISA6
  _TRISA6 = 1;
#endif
#ifdef _ANSA6
  _ANSA6 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA6
#ifdef _RA7
#ifdef _CNPUA7
#define ENABLE_RA7_PULLUP() _CNPUA7 = 1
#define DISABLE_RA7_PULLUP() _CNPUA7 = 0
#endif
#ifdef _CNPDA7
#define ENABLE_RA7_PULLDOWN() _CNPDA7 = 1
#define DISABLE_RA7_PULLDOWN() _CNPDA7 = 0
#endif
#ifdef _ANSA7
#define ENABLE_RA7_ANALOG() _ANSA7 = 1
#define DISABLE_RA7_ANALOG() _ANSA7 = 0
#endif
#ifdef _CNIEA7
#define ENABLE_RA7_CN_INTERRUPT() _CNIEA7 = 1
#define DISABLE_RA7_CN_INTERRUPT() _CNIEA7 = 0
#endif
static inline void CONFIG_RA7_AS_DIG_OUTPUT() {
#ifdef _CNPUA7
  _CNPUA7 = 0; //disable pullup
#endif
#ifdef _CNPDA7
  _CNPDA7 = 0; //disable pulldown
#endif
#ifdef _ODCA7
  _ODCA7 = 0;
#endif
#ifdef _TRISA7
  _TRISA7 = 0;
#endif
#ifdef _ANSA7
  _ANSA7 = 0;  //disable analog
#endif
}
#ifdef _ODCA7
#define ENABLE_RA7_OPENDRAIN() _ODCA7 = 1
#define DISABLE_RA7_OPENDRAIN() _ODCA7 = 0
#define CONFIG_RA7_AS_DIG_OD_OUTPUT()  {CONFIG_RA7_AS_DIG_OUTPUT();ENABLE_RA7_OPENDRAIN();}
#endif
static inline void CONFIG_RA7_AS_DIG_INPUT() {
#ifdef _CNPUA7
  _CNPUA7 = 0; //disable pullup
#endif
#ifdef _CNPDA7
  _CNPDA7 = 0; //disable pulldown
#endif
#ifdef _TRISA7
  _TRISA7 = 1;
#endif
#ifdef _ANSA7
  _ANSA7 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA7
#ifdef _RA8
#ifdef _CNPUA8
#define ENABLE_RA8_PULLUP() _CNPUA8 = 1
#define DISABLE_RA8_PULLUP() _CNPUA8 = 0
#endif
#ifdef _CNPDA8
#define ENABLE_RA8_PULLDOWN() _CNPDA8 = 1
#define DISABLE_RA8_PULLDOWN() _CNPDA8 = 0
#endif
#ifdef _ANSA8
#define ENABLE_RA8_ANALOG() _ANSA8 = 1
#define DISABLE_RA8_ANALOG() _ANSA8 = 0
#endif
#ifdef _CNIEA8
#define ENABLE_RA8_CN_INTERRUPT() _CNIEA8 = 1
#define DISABLE_RA8_CN_INTERRUPT() _CNIEA8 = 0
#endif
static inline void CONFIG_RA8_AS_DIG_OUTPUT() {
#ifdef _CNPUA8
  _CNPUA8 = 0; //disable pullup
#endif
#ifdef _CNPDA8
  _CNPDA8 = 0; //disable pulldown
#endif
#ifdef _ODCA8
  _ODCA8 = 0;
#endif
#ifdef _TRISA8
  _TRISA8 = 0;
#endif
#ifdef _ANSA8
  _ANSA8 = 0;  //disable analog
#endif
}
#ifdef _ODCA8
#define ENABLE_RA8_OPENDRAIN() _ODCA8 = 1
#define DISABLE_RA8_OPENDRAIN() _ODCA8 = 0
#define CONFIG_RA8_AS_DIG_OD_OUTPUT()  {CONFIG_RA8_AS_DIG_OUTPUT();ENABLE_RA8_OPENDRAIN();}
#endif
static inline void CONFIG_RA8_AS_DIG_INPUT() {
#ifdef _CNPUA8
  _CNPUA8 = 0; //disable pullup
#endif
#ifdef _CNPDA8
  _CNPDA8 = 0; //disable pulldown
#endif
#ifdef _TRISA8
  _TRISA8 = 1;
#endif
#ifdef _ANSA8
  _ANSA8 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA8
#ifdef _RA9
#ifdef _CNPUA9
#define ENABLE_RA9_PULLUP() _CNPUA9 = 1
#define DISABLE_RA9_PULLUP() _CNPUA9 = 0
#endif
#ifdef _CNPDA9
#define ENABLE_RA9_PULLDOWN() _CNPDA9 = 1
#define DISABLE_RA9_PULLDOWN() _CNPDA9 = 0
#endif
#ifdef _ANSA9
#define ENABLE_RA9_ANALOG() _ANSA9 = 1
#define DISABLE_RA9_ANALOG() _ANSA9 = 0
#endif
#ifdef _CNIEA9
#define ENABLE_RA9_CN_INTERRUPT() _CNIEA9 = 1
#define DISABLE_RA9_CN_INTERRUPT() _CNIEA9 = 0
#endif
static inline void CONFIG_RA9_AS_DIG_OUTPUT() {
#ifdef _CNPUA9
  _CNPUA9 = 0; //disable pullup
#endif
#ifdef _CNPDA9
  _CNPDA9 = 0; //disable pulldown
#endif
#ifdef _ODCA9
  _ODCA9 = 0;
#endif
#ifdef _TRISA9
  _TRISA9 = 0;
#endif
#ifdef _ANSA9
  _ANSA9 = 0;  //disable analog
#endif
}
#ifdef _ODCA9
#define ENABLE_RA9_OPENDRAIN() _ODCA9 = 1
#define DISABLE_RA9_OPENDRAIN() _ODCA9 = 0
#define CONFIG_RA9_AS_DIG_OD_OUTPUT()  {CONFIG_RA9_AS_DIG_OUTPUT();ENABLE_RA9_OPENDRAIN();}
#endif
static inline void CONFIG_RA9_AS_DIG_INPUT() {
#ifdef _CNPUA9
  _CNPUA9 = 0; //disable pullup
#endif
#ifdef _CNPDA9
  _CNPDA9 = 0; //disable pulldown
#endif
#ifdef _TRISA9
  _TRISA9 = 1;
#endif
#ifdef _ANSA9
  _ANSA9 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA9
#ifdef _RA10
#ifdef _CNPUA10
#define ENABLE_RA10_PULLUP() _CNPUA10 = 1
#define DISABLE_RA10_PULLUP() _CNPUA10 = 0
#endif
#ifdef _CNPDA10
#define ENABLE_RA10_PULLDOWN() _CNPDA10 = 1
#define DISABLE_RA10_PULLDOWN() _CNPDA10 = 0
#endif
#ifdef _ANSA10
#define ENABLE_RA10_ANALOG() _ANSA10 = 1
#define DISABLE_RA10_ANALOG() _ANSA10 = 0
#endif
#ifdef _CNIEA10
#define ENABLE_RA10_CN_INTERRUPT() _CNIEA10 = 1
#define DISABLE_RA10_CN_INTERRUPT() _CNIEA10 = 0
#endif
static inline void CONFIG_RA10_AS_DIG_OUTPUT() {
#ifdef _CNPUA10
  _CNPUA10 = 0; //disable pullup
#endif
#ifdef _CNPDA10
  _CNPDA10 = 0; //disable pulldown
#endif
#ifdef _ODCA10
  _ODCA10 = 0;
#endif
#ifdef _TRISA10
  _TRISA10 = 0;
#endif
#ifdef _ANSA10
  _ANSA10 = 0;  //disable analog
#endif
}
#ifdef _ODCA10
#define ENABLE_RA10_OPENDRAIN() _ODCA10 = 1
#define DISABLE_RA10_OPENDRAIN() _ODCA10 = 0
#define CONFIG_RA10_AS_DIG_OD_OUTPUT()  {CONFIG_RA10_AS_DIG_OUTPUT();ENABLE_RA10_OPENDRAIN();}
#endif
static inline void CONFIG_RA10_AS_DIG_INPUT() {
#ifdef _CNPUA10
  _CNPUA10 = 0; //disable pullup
#endif
#ifdef _CNPDA10
  _CNPDA10 = 0; //disable pulldown
#endif
#ifdef _TRISA10
  _TRISA10 = 1;
#endif
#ifdef _ANSA10
  _ANSA10 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA10
#ifdef _RA11
#ifdef _CNPUA11
#define ENABLE_RA11_PULLUP() _CNPUA11 = 1
#define DISABLE_RA11_PULLUP() _CNPUA11 = 0
#endif
#ifdef _CNPDA11
#define ENABLE_RA11_PULLDOWN() _CNPDA11 = 1
#define DISABLE_RA11_PULLDOWN() _CNPDA11 = 0
#endif
#ifdef _ANSA11
#define ENABLE_RA11_ANALOG() _ANSA11 = 1
#define DISABLE_RA11_ANALOG() _ANSA11 = 0
#endif
#ifdef _CNIEA11
#define ENABLE_RA11_CN_INTERRUPT() _CNIEA11 = 1
#define DISABLE_RA11_CN_INTERRUPT() _CNIEA11 = 0
#endif
static inline void CONFIG_RA11_AS_DIG_OUTPUT() {
#ifdef _CNPUA11
  _CNPUA11 = 0; //disable pullup
#endif
#ifdef _CNPDA11
  _CNPDA11 = 0; //disable pulldown
#endif
#ifdef _ODCA11
  _ODCA11 = 0;
#endif
#ifdef _TRISA11
  _TRISA11 = 0;
#endif
#ifdef _ANSA11
  _ANSA11 = 0;  //disable analog
#endif
}
#ifdef _ODCA11
#define ENABLE_RA11_OPENDRAIN() _ODCA11 = 1
#define DISABLE_RA11_OPENDRAIN() _ODCA11 = 0
#define CONFIG_RA11_AS_DIG_OD_OUTPUT()  {CONFIG_RA11_AS_DIG_OUTPUT();ENABLE_RA11_OPENDRAIN();}
#endif
static inline void CONFIG_RA11_AS_DIG_INPUT() {
#ifdef _CNPUA11
  _CNPUA11 = 0; //disable pullup
#endif
#ifdef _CNPDA11
  _CNPDA11 = 0; //disable pulldown
#endif
#ifdef _TRISA11
  _TRISA11 = 1;
#endif
#ifdef _ANSA11
  _ANSA11 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA11
#ifdef _RA12
#ifdef _CNPUA12
#define ENABLE_RA12_PULLUP() _CNPUA12 = 1
#define DISABLE_RA12_PULLUP() _CNPUA12 = 0
#endif
#ifdef _CNPDA12
#define ENABLE_RA12_PULLDOWN() _CNPDA12 = 1
#define DISABLE_RA12_PULLDOWN() _CNPDA12 = 0
#endif
#ifdef _ANSA12
#define ENABLE_RA12_ANALOG() _ANSA12 = 1
#define DISABLE_RA12_ANALOG() _ANSA12 = 0
#endif
#ifdef _CNIEA12
#define ENABLE_RA12_CN_INTERRUPT() _CNIEA12 = 1
#define DISABLE_RA12_CN_INTERRUPT() _CNIEA12 = 0
#endif
static inline void CONFIG_RA12_AS_DIG_OUTPUT() {
#ifdef _CNPUA12
  _CNPUA12 = 0; //disable pullup
#endif
#ifdef _CNPDA12
  _CNPDA12 = 0; //disable pulldown
#endif
#ifdef _ODCA12
  _ODCA12 = 0;
#endif
#ifdef _TRISA12
  _TRISA12 = 0;
#endif
#ifdef _ANSA12
  _ANSA12 = 0;  //disable analog
#endif
}
#ifdef _ODCA12
#define ENABLE_RA12_OPENDRAIN() _ODCA12 = 1
#define DISABLE_RA12_OPENDRAIN() _ODCA12 = 0
#define CONFIG_RA12_AS_DIG_OD_OUTPUT()  {CONFIG_RA12_AS_DIG_OUTPUT();ENABLE_RA12_OPENDRAIN();}
#endif
static inline void CONFIG_RA12_AS_DIG_INPUT() {
#ifdef _CNPUA12
  _CNPUA12 = 0; //disable pullup
#endif
#ifdef _CNPDA12
  _CNPDA12 = 0; //disable pulldown
#endif
#ifdef _TRISA12
  _TRISA12 = 1;
#endif
#ifdef _ANSA12
  _ANSA12 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA12
#ifdef _RA13
#ifdef _CNPUA13
#define ENABLE_RA13_PULLUP() _CNPUA13 = 1
#define DISABLE_RA13_PULLUP() _CNPUA13 = 0
#endif
#ifdef _CNPDA13
#define ENABLE_RA13_PULLDOWN() _CNPDA13 = 1
#define DISABLE_RA13_PULLDOWN() _CNPDA13 = 0
#endif
#ifdef _ANSA13
#define ENABLE_RA13_ANALOG() _ANSA13 = 1
#define DISABLE_RA13_ANALOG() _ANSA13 = 0
#endif
#ifdef _CNIEA13
#define ENABLE_RA13_CN_INTERRUPT() _CNIEA13 = 1
#define DISABLE_RA13_CN_INTERRUPT() _CNIEA13 = 0
#endif
static inline void CONFIG_RA13_AS_DIG_OUTPUT() {
#ifdef _CNPUA13
  _CNPUA13 = 0; //disable pullup
#endif
#ifdef _CNPDA13
  _CNPDA13 = 0; //disable pulldown
#endif
#ifdef _ODCA13
  _ODCA13 = 0;
#endif
#ifdef _TRISA13
  _TRISA13 = 0;
#endif
#ifdef _ANSA13
  _ANSA13 = 0;  //disable analog
#endif
}
#ifdef _ODCA13
#define ENABLE_RA13_OPENDRAIN() _ODCA13 = 1
#define DISABLE_RA13_OPENDRAIN() _ODCA13 = 0
#define CONFIG_RA13_AS_DIG_OD_OUTPUT()  {CONFIG_RA13_AS_DIG_OUTPUT();ENABLE_RA13_OPENDRAIN();}
#endif
static inline void CONFIG_RA13_AS_DIG_INPUT() {
#ifdef _CNPUA13
  _CNPUA13 = 0; //disable pullup
#endif
#ifdef _CNPDA13
  _CNPDA13 = 0; //disable pulldown
#endif
#ifdef _TRISA13
  _TRISA13 = 1;
#endif
#ifdef _ANSA13
  _ANSA13 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA13
#ifdef _RA14
#ifdef _CNPUA14
#define ENABLE_RA14_PULLUP() _CNPUA14 = 1
#define DISABLE_RA14_PULLUP() _CNPUA14 = 0
#endif
#ifdef _CNPDA14
#define ENABLE_RA14_PULLDOWN() _CNPDA14 = 1
#define DISABLE_RA14_PULLDOWN() _CNPDA14 = 0
#endif
#ifdef _ANSA14
#define ENABLE_RA14_ANALOG() _ANSA14 = 1
#define DISABLE_RA14_ANALOG() _ANSA14 = 0
#endif
#ifdef _CNIEA14
#define ENABLE_RA14_CN_INTERRUPT() _CNIEA14 = 1
#define DISABLE_RA14_CN_INTERRUPT() _CNIEA14 = 0
#endif
static inline void CONFIG_RA14_AS_DIG_OUTPUT() {
#ifdef _CNPUA14
  _CNPUA14 = 0; //disable pullup
#endif
#ifdef _CNPDA14
  _CNPDA14 = 0; //disable pulldown
#endif
#ifdef _ODCA14
  _ODCA14 = 0;
#endif
#ifdef _TRISA14
  _TRISA14 = 0;
#endif
#ifdef _ANSA14
  _ANSA14 = 0;  //disable analog
#endif
}
#ifdef _ODCA14
#define ENABLE_RA14_OPENDRAIN() _ODCA14 = 1
#define DISABLE_RA14_OPENDRAIN() _ODCA14 = 0
#define CONFIG_RA14_AS_DIG_OD_OUTPUT()  {CONFIG_RA14_AS_DIG_OUTPUT();ENABLE_RA14_OPENDRAIN();}
#endif
static inline void CONFIG_RA14_AS_DIG_INPUT() {
#ifdef _CNPUA14
  _CNPUA14 = 0; //disable pullup
#endif
#ifdef _CNPDA14
  _CNPDA14 = 0; //disable pulldown
#endif
#ifdef _TRISA14
  _TRISA14 = 1;
#endif
#ifdef _ANSA14
  _ANSA14 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA14
#ifdef _RA15
#ifdef _CNPUA15
#define ENABLE_RA15_PULLUP() _CNPUA15 = 1
#define DISABLE_RA15_PULLUP() _CNPUA15 = 0
#endif
#ifdef _CNPDA15
#define ENABLE_RA15_PULLDOWN() _CNPDA15 = 1
#define DISABLE_RA15_PULLDOWN() _CNPDA15 = 0
#endif
#ifdef _ANSA15
#define ENABLE_RA15_ANALOG() _ANSA15 = 1
#define DISABLE_RA15_ANALOG() _ANSA15 = 0
#endif
#ifdef _CNIEA15
#define ENABLE_RA15_CN_INTERRUPT() _CNIEA15 = 1
#define DISABLE_RA15_CN_INTERRUPT() _CNIEA15 = 0
#endif
static inline void CONFIG_RA15_AS_DIG_OUTPUT() {
#ifdef _CNPUA15
  _CNPUA15 = 0; //disable pullup
#endif
#ifdef _CNPDA15
  _CNPDA15 = 0; //disable pulldown
#endif
#ifdef _ODCA15
  _ODCA15 = 0;
#endif
#ifdef _TRISA15
  _TRISA15 = 0;
#endif
#ifdef _ANSA15
  _ANSA15 = 0;  //disable analog
#endif
}
#ifdef _ODCA15
#define ENABLE_RA15_OPENDRAIN() _ODCA15 = 1
#define DISABLE_RA15_OPENDRAIN() _ODCA15 = 0
#define CONFIG_RA15_AS_DIG_OD_OUTPUT()  {CONFIG_RA15_AS_DIG_OUTPUT();ENABLE_RA15_OPENDRAIN();}
#endif
static inline void CONFIG_RA15_AS_DIG_INPUT() {
#ifdef _CNPUA15
  _CNPUA15 = 0; //disable pullup
#endif
#ifdef _CNPDA15
  _CNPDA15 = 0; //disable pulldown
#endif
#ifdef _TRISA15
  _TRISA15 = 1;
#endif
#ifdef _ANSA15
  _ANSA15 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RA15
#ifdef _RB0
#ifdef _CNPUB0
#define ENABLE_RB0_PULLUP() _CNPUB0 = 1
#define DISABLE_RB0_PULLUP() _CNPUB0 = 0
#endif
#ifdef _CNPDB0
#define ENABLE_RB0_PULLDOWN() _CNPDB0 = 1
#define DISABLE_RB0_PULLDOWN() _CNPDB0 = 0
#endif
#ifdef _ANSB0
#define ENABLE_RB0_ANALOG() _ANSB0 = 1
#define DISABLE_RB0_ANALOG() _ANSB0 = 0
#endif
#ifdef _CNIEB0
#define ENABLE_RB0_CN_INTERRUPT() _CNIEB0 = 1
#define DISABLE_RB0_CN_INTERRUPT() _CNIEB0 = 0
#endif
static inline void CONFIG_RB0_AS_DIG_OUTPUT() {
#ifdef _CNPUB0
  _CNPUB0 = 0; //disable pullup
#endif
#ifdef _CNPDB0
  _CNPDB0 = 0; //disable pulldown
#endif
#ifdef _ODCB0
  _ODCB0 = 0;
#endif
#ifdef _TRISB0
  _TRISB0 = 0;
#endif
#ifdef _ANSB0
  _ANSB0 = 0;  //disable analog
#endif
}
#ifdef _ODCB0
#define ENABLE_RB0_OPENDRAIN() _ODCB0 = 1
#define DISABLE_RB0_OPENDRAIN() _ODCB0 = 0
#define CONFIG_RB0_AS_DIG_OD_OUTPUT()  {CONFIG_RB0_AS_DIG_OUTPUT();ENABLE_RB0_OPENDRAIN();}
#endif
static inline void CONFIG_RB0_AS_DIG_INPUT() {
#ifdef _CNPUB0
  _CNPUB0 = 0; //disable pullup
#endif
#ifdef _CNPDB0
  _CNPDB0 = 0; //disable pulldown
#endif
#ifdef _TRISB0
  _TRISB0 = 1;
#endif
#ifdef _ANSB0
  _ANSB0 = 0;  //disable analog
#endif
}
#define CONFIG_AN0_AS_DIGITAL() _ANSB0 = 0
#define CONFIG_AN0_AS_ANALOG() _ANSB0 = 1
#endif  //endif #ifdef _RB0
#ifdef _RB1
#ifdef _CNPUB1
#define ENABLE_RB1_PULLUP() _CNPUB1 = 1
#define DISABLE_RB1_PULLUP() _CNPUB1 = 0
#endif
#ifdef _CNPDB1
#define ENABLE_RB1_PULLDOWN() _CNPDB1 = 1
#define DISABLE_RB1_PULLDOWN() _CNPDB1 = 0
#endif
#ifdef _ANSB1
#define ENABLE_RB1_ANALOG() _ANSB1 = 1
#define DISABLE_RB1_ANALOG() _ANSB1 = 0
#endif
#ifdef _CNIEB1
#define ENABLE_RB1_CN_INTERRUPT() _CNIEB1 = 1
#define DISABLE_RB1_CN_INTERRUPT() _CNIEB1 = 0
#endif
static inline void CONFIG_RB1_AS_DIG_OUTPUT() {
#ifdef _CNPUB1
  _CNPUB1 = 0; //disable pullup
#endif
#ifdef _CNPDB1
  _CNPDB1 = 0; //disable pulldown
#endif
#ifdef _ODCB1
  _ODCB1 = 0;
#endif
#ifdef _TRISB1
  _TRISB1 = 0;
#endif
#ifdef _ANSB1
  _ANSB1 = 0;  //disable analog
#endif
}
#ifdef _ODCB1
#define ENABLE_RB1_OPENDRAIN() _ODCB1 = 1
#define DISABLE_RB1_OPENDRAIN() _ODCB1 = 0
#define CONFIG_RB1_AS_DIG_OD_OUTPUT()  {CONFIG_RB1_AS_DIG_OUTPUT();ENABLE_RB1_OPENDRAIN();}
#endif
static inline void CONFIG_RB1_AS_DIG_INPUT() {
#ifdef _CNPUB1
  _CNPUB1 = 0; //disable pullup
#endif
#ifdef _CNPDB1
  _CNPDB1 = 0; //disable pulldown
#endif
#ifdef _TRISB1
  _TRISB1 = 1;
#endif
#ifdef _ANSB1
  _ANSB1 = 0;  //disable analog
#endif
}
#define CONFIG_AN1_AS_DIGITAL() _ANSB1 = 0
#define CONFIG_AN1_AS_ANALOG() _ANSB1 = 1
#endif  //endif #ifdef _RB1
#ifdef _RB2
#ifdef _CNPUB2
#define ENABLE_RB2_PULLUP() _CNPUB2 = 1
#define DISABLE_RB2_PULLUP() _CNPUB2 = 0
#endif
#ifdef _CNPDB2
#define ENABLE_RB2_PULLDOWN() _CNPDB2 = 1
#define DISABLE_RB2_PULLDOWN() _CNPDB2 = 0
#endif
#ifdef _ANSB2
#define ENABLE_RB2_ANALOG() _ANSB2 = 1
#define DISABLE_RB2_ANALOG() _ANSB2 = 0
#endif
#ifdef _CNIEB2
#define ENABLE_RB2_CN_INTERRUPT() _CNIEB2 = 1
#define DISABLE_RB2_CN_INTERRUPT() _CNIEB2 = 0
#endif
static inline void CONFIG_RB2_AS_DIG_OUTPUT() {
#ifdef _CNPUB2
  _CNPUB2 = 0; //disable pullup
#endif
#ifdef _CNPDB2
  _CNPDB2 = 0; //disable pulldown
#endif
#ifdef _ODCB2
  _ODCB2 = 0;
#endif
#ifdef _TRISB2
  _TRISB2 = 0;
#endif
#ifdef _ANSB2
  _ANSB2 = 0;  //disable analog
#endif
}
#ifdef _ODCB2
#define ENABLE_RB2_OPENDRAIN() _ODCB2 = 1
#define DISABLE_RB2_OPENDRAIN() _ODCB2 = 0
#define CONFIG_RB2_AS_DIG_OD_OUTPUT()  {CONFIG_RB2_AS_DIG_OUTPUT();ENABLE_RB2_OPENDRAIN();}
#endif
static inline void CONFIG_RB2_AS_DIG_INPUT() {
#ifdef _CNPUB2
  _CNPUB2 = 0; //disable pullup
#endif
#ifdef _CNPDB2
  _CNPDB2 = 0; //disable pulldown
#endif
#ifdef _TRISB2
  _TRISB2 = 1;
#endif
#ifdef _ANSB2
  _ANSB2 = 0;  //disable analog
#endif
}
#define CONFIG_AN2_AS_DIGITAL() _ANSB2 = 0
#define CONFIG_AN2_AS_ANALOG() _ANSB2 = 1
#endif  //endif #ifdef _RB2
#ifdef _RB3
#ifdef _CNPUB3
#define ENABLE_RB3_PULLUP() _CNPUB3 = 1
#define DISABLE_RB3_PULLUP() _CNPUB3 = 0
#endif
#ifdef _CNPDB3
#define ENABLE_RB3_PULLDOWN() _CNPDB3 = 1
#define DISABLE_RB3_PULLDOWN() _CNPDB3 = 0
#endif
#ifdef _ANSB3
#define ENABLE_RB3_ANALOG() _ANSB3 = 1
#define DISABLE_RB3_ANALOG() _ANSB3 = 0
#endif
#ifdef _CNIEB3
#define ENABLE_RB3_CN_INTERRUPT() _CNIEB3 = 1
#define DISABLE_RB3_CN_INTERRUPT() _CNIEB3 = 0
#endif
static inline void CONFIG_RB3_AS_DIG_OUTPUT() {
#ifdef _CNPUB3
  _CNPUB3 = 0; //disable pullup
#endif
#ifdef _CNPDB3
  _CNPDB3 = 0; //disable pulldown
#endif
#ifdef _ODCB3
  _ODCB3 = 0;
#endif
#ifdef _TRISB3
  _TRISB3 = 0;
#endif
#ifdef _ANSB3
  _ANSB3 = 0;  //disable analog
#endif
}
#ifdef _ODCB3
#define ENABLE_RB3_OPENDRAIN() _ODCB3 = 1
#define DISABLE_RB3_OPENDRAIN() _ODCB3 = 0
#define CONFIG_RB3_AS_DIG_OD_OUTPUT()  {CONFIG_RB3_AS_DIG_OUTPUT();ENABLE_RB3_OPENDRAIN();}
#endif
static inline void CONFIG_RB3_AS_DIG_INPUT() {
#ifdef _CNPUB3
  _CNPUB3 = 0; //disable pullup
#endif
#ifdef _CNPDB3
  _CNPDB3 = 0; //disable pulldown
#endif
#ifdef _TRISB3
  _TRISB3 = 1;
#endif
#ifdef _ANSB3
  _ANSB3 = 0;  //disable analog
#endif
}
#define CONFIG_AN3_AS_DIGITAL() _ANSB3 = 0
#define CONFIG_AN3_AS_ANALOG() _ANSB3 = 1
#endif  //endif #ifdef _RB3
#ifdef _RB4
#ifdef _CNPUB4
#define ENABLE_RB4_PULLUP() _CNPUB4 = 1
#define DISABLE_RB4_PULLUP() _CNPUB4 = 0
#endif
#ifdef _CNPDB4
#define ENABLE_RB4_PULLDOWN() _CNPDB4 = 1
#define DISABLE_RB4_PULLDOWN() _CNPDB4 = 0
#endif
#ifdef _ANSB4
#define ENABLE_RB4_ANALOG() _ANSB4 = 1
#define DISABLE_RB4_ANALOG() _ANSB4 = 0
#endif
#ifdef _CNIEB4
#define ENABLE_RB4_CN_INTERRUPT() _CNIEB4 = 1
#define DISABLE_RB4_CN_INTERRUPT() _CNIEB4 = 0
#endif
static inline void CONFIG_RB4_AS_DIG_OUTPUT() {
#ifdef _CNPUB4
  _CNPUB4 = 0; //disable pullup
#endif
#ifdef _CNPDB4
  _CNPDB4 = 0; //disable pulldown
#endif
#ifdef _ODCB4
  _ODCB4 = 0;
#endif
#ifdef _TRISB4
  _TRISB4 = 0;
#endif
#ifdef _ANSB4
  _ANSB4 = 0;  //disable analog
#endif
}
#ifdef _ODCB4
#define ENABLE_RB4_OPENDRAIN() _ODCB4 = 1
#define DISABLE_RB4_OPENDRAIN() _ODCB4 = 0
#define CONFIG_RB4_AS_DIG_OD_OUTPUT()  {CONFIG_RB4_AS_DIG_OUTPUT();ENABLE_RB4_OPENDRAIN();}
#endif
static inline void CONFIG_RB4_AS_DIG_INPUT() {
#ifdef _CNPUB4
  _CNPUB4 = 0; //disable pullup
#endif
#ifdef _CNPDB4
  _CNPDB4 = 0; //disable pulldown
#endif
#ifdef _TRISB4
  _TRISB4 = 1;
#endif
#ifdef _ANSB4
  _ANSB4 = 0;  //disable analog
#endif
}
#define CONFIG_AN4_AS_DIGITAL() _ANSB4 = 0
#define CONFIG_AN4_AS_ANALOG() _ANSB4 = 1
#endif  //endif #ifdef _RB4
#ifdef _RB5
#ifdef _CNPUB5
#define ENABLE_RB5_PULLUP() _CNPUB5 = 1
#define DISABLE_RB5_PULLUP() _CNPUB5 = 0
#endif
#ifdef _CNPDB5
#define ENABLE_RB5_PULLDOWN() _CNPDB5 = 1
#define DISABLE_RB5_PULLDOWN() _CNPDB5 = 0
#endif
#ifdef _ANSB5
#define ENABLE_RB5_ANALOG() _ANSB5 = 1
#define DISABLE_RB5_ANALOG() _ANSB5 = 0
#endif
#ifdef _CNIEB5
#define ENABLE_RB5_CN_INTERRUPT() _CNIEB5 = 1
#define DISABLE_RB5_CN_INTERRUPT() _CNIEB5 = 0
#endif
static inline void CONFIG_RB5_AS_DIG_OUTPUT() {
#ifdef _CNPUB5
  _CNPUB5 = 0; //disable pullup
#endif
#ifdef _CNPDB5
  _CNPDB5 = 0; //disable pulldown
#endif
#ifdef _ODCB5
  _ODCB5 = 0;
#endif
#ifdef _TRISB5
  _TRISB5 = 0;
#endif
#ifdef _ANSB5
  _ANSB5 = 0;  //disable analog
#endif
}
#ifdef _ODCB5
#define ENABLE_RB5_OPENDRAIN() _ODCB5 = 1
#define DISABLE_RB5_OPENDRAIN() _ODCB5 = 0
#define CONFIG_RB5_AS_DIG_OD_OUTPUT()  {CONFIG_RB5_AS_DIG_OUTPUT();ENABLE_RB5_OPENDRAIN();}
#endif
static inline void CONFIG_RB5_AS_DIG_INPUT() {
#ifdef _CNPUB5
  _CNPUB5 = 0; //disable pullup
#endif
#ifdef _CNPDB5
  _CNPDB5 = 0; //disable pulldown
#endif
#ifdef _TRISB5
  _TRISB5 = 1;
#endif
#ifdef _ANSB5
  _ANSB5 = 0;  //disable analog
#endif
}
#define CONFIG_AN5_AS_DIGITAL() _ANSB5 = 0
#define CONFIG_AN5_AS_ANALOG() _ANSB5 = 1
#endif  //endif #ifdef _RB5
#ifdef _RB6
#ifdef _CNPUB6
#define ENABLE_RB6_PULLUP() _CNPUB6 = 1
#define DISABLE_RB6_PULLUP() _CNPUB6 = 0
#endif
#ifdef _CNPDB6
#define ENABLE_RB6_PULLDOWN() _CNPDB6 = 1
#define DISABLE_RB6_PULLDOWN() _CNPDB6 = 0
#endif
#ifdef _ANSB6
#define ENABLE_RB6_ANALOG() _ANSB6 = 1
#define DISABLE_RB6_ANALOG() _ANSB6 = 0
#endif
#ifdef _CNIEB6
#define ENABLE_RB6_CN_INTERRUPT() _CNIEB6 = 1
#define DISABLE_RB6_CN_INTERRUPT() _CNIEB6 = 0
#endif
static inline void CONFIG_RB6_AS_DIG_OUTPUT() {
#ifdef _CNPUB6
  _CNPUB6 = 0; //disable pullup
#endif
#ifdef _CNPDB6
  _CNPDB6 = 0; //disable pulldown
#endif
#ifdef _ODCB6
  _ODCB6 = 0;
#endif
#ifdef _TRISB6
  _TRISB6 = 0;
#endif
#ifdef _ANSB6
  _ANSB6 = 0;  //disable analog
#endif
}
#ifdef _ODCB6
#define ENABLE_RB6_OPENDRAIN() _ODCB6 = 1
#define DISABLE_RB6_OPENDRAIN() _ODCB6 = 0
#define CONFIG_RB6_AS_DIG_OD_OUTPUT()  {CONFIG_RB6_AS_DIG_OUTPUT();ENABLE_RB6_OPENDRAIN();}
#endif
static inline void CONFIG_RB6_AS_DIG_INPUT() {
#ifdef _CNPUB6
  _CNPUB6 = 0; //disable pullup
#endif
#ifdef _CNPDB6
  _CNPDB6 = 0; //disable pulldown
#endif
#ifdef _TRISB6
  _TRISB6 = 1;
#endif
#ifdef _ANSB6
  _ANSB6 = 0;  //disable analog
#endif
}
#define CONFIG_AN6_AS_DIGITAL() _ANSB6 = 0
#define CONFIG_AN6_AS_ANALOG() _ANSB6 = 1
#endif  //endif #ifdef _RB6
#ifdef _RB7
#ifdef _CNPUB7
#define ENABLE_RB7_PULLUP() _CNPUB7 = 1
#define DISABLE_RB7_PULLUP() _CNPUB7 = 0
#endif
#ifdef _CNPDB7
#define ENABLE_RB7_PULLDOWN() _CNPDB7 = 1
#define DISABLE_RB7_PULLDOWN() _CNPDB7 = 0
#endif
#ifdef _ANSB7
#define ENABLE_RB7_ANALOG() _ANSB7 = 1
#define DISABLE_RB7_ANALOG() _ANSB7 = 0
#endif
#ifdef _CNIEB7
#define ENABLE_RB7_CN_INTERRUPT() _CNIEB7 = 1
#define DISABLE_RB7_CN_INTERRUPT() _CNIEB7 = 0
#endif
static inline void CONFIG_RB7_AS_DIG_OUTPUT() {
#ifdef _CNPUB7
  _CNPUB7 = 0; //disable pullup
#endif
#ifdef _CNPDB7
  _CNPDB7 = 0; //disable pulldown
#endif
#ifdef _ODCB7
  _ODCB7 = 0;
#endif
#ifdef _TRISB7
  _TRISB7 = 0;
#endif
#ifdef _ANSB7
  _ANSB7 = 0;  //disable analog
#endif
}
#ifdef _ODCB7
#define ENABLE_RB7_OPENDRAIN() _ODCB7 = 1
#define DISABLE_RB7_OPENDRAIN() _ODCB7 = 0
#define CONFIG_RB7_AS_DIG_OD_OUTPUT()  {CONFIG_RB7_AS_DIG_OUTPUT();ENABLE_RB7_OPENDRAIN();}
#endif
static inline void CONFIG_RB7_AS_DIG_INPUT() {
#ifdef _CNPUB7
  _CNPUB7 = 0; //disable pullup
#endif
#ifdef _CNPDB7
  _CNPDB7 = 0; //disable pulldown
#endif
#ifdef _TRISB7
  _TRISB7 = 1;
#endif
#ifdef _ANSB7
  _ANSB7 = 0;  //disable analog
#endif
}
#define CONFIG_AN7_AS_DIGITAL() _ANSB7 = 0
#define CONFIG_AN7_AS_ANALOG() _ANSB7 = 1
#endif  //endif #ifdef _RB7
#ifdef _RB8
#ifdef _CNPUB8
#define ENABLE_RB8_PULLUP() _CNPUB8 = 1
#define DISABLE_RB8_PULLUP() _CNPUB8 = 0
#endif
#ifdef _CNPDB8
#define ENABLE_RB8_PULLDOWN() _CNPDB8 = 1
#define DISABLE_RB8_PULLDOWN() _CNPDB8 = 0
#endif
#ifdef _ANSB8
#define ENABLE_RB8_ANALOG() _ANSB8 = 1
#define DISABLE_RB8_ANALOG() _ANSB8 = 0
#endif
#ifdef _CNIEB8
#define ENABLE_RB8_CN_INTERRUPT() _CNIEB8 = 1
#define DISABLE_RB8_CN_INTERRUPT() _CNIEB8 = 0
#endif
static inline void CONFIG_RB8_AS_DIG_OUTPUT() {
#ifdef _CNPUB8
  _CNPUB8 = 0; //disable pullup
#endif
#ifdef _CNPDB8
  _CNPDB8 = 0; //disable pulldown
#endif
#ifdef _ODCB8
  _ODCB8 = 0;
#endif
#ifdef _TRISB8
  _TRISB8 = 0;
#endif
#ifdef _ANSB8
  _ANSB8 = 0;  //disable analog
#endif
}
#ifdef _ODCB8
#define ENABLE_RB8_OPENDRAIN() _ODCB8 = 1
#define DISABLE_RB8_OPENDRAIN() _ODCB8 = 0
#define CONFIG_RB8_AS_DIG_OD_OUTPUT()  {CONFIG_RB8_AS_DIG_OUTPUT();ENABLE_RB8_OPENDRAIN();}
#endif
static inline void CONFIG_RB8_AS_DIG_INPUT() {
#ifdef _CNPUB8
  _CNPUB8 = 0; //disable pullup
#endif
#ifdef _CNPDB8
  _CNPDB8 = 0; //disable pulldown
#endif
#ifdef _TRISB8
  _TRISB8 = 1;
#endif
#ifdef _ANSB8
  _ANSB8 = 0;  //disable analog
#endif
}
#define CONFIG_AN8_AS_DIGITAL() _ANSB8 = 0
#define CONFIG_AN8_AS_ANALOG() _ANSB8 = 1
#endif  //endif #ifdef _RB8
#ifdef _RB9
#ifdef _CNPUB9
#define ENABLE_RB9_PULLUP() _CNPUB9 = 1
#define DISABLE_RB9_PULLUP() _CNPUB9 = 0
#endif
#ifdef _CNPDB9
#define ENABLE_RB9_PULLDOWN() _CNPDB9 = 1
#define DISABLE_RB9_PULLDOWN() _CNPDB9 = 0
#endif
#ifdef _ANSB9
#define ENABLE_RB9_ANALOG() _ANSB9 = 1
#define DISABLE_RB9_ANALOG() _ANSB9 = 0
#endif
#ifdef _CNIEB9
#define ENABLE_RB9_CN_INTERRUPT() _CNIEB9 = 1
#define DISABLE_RB9_CN_INTERRUPT() _CNIEB9 = 0
#endif
static inline void CONFIG_RB9_AS_DIG_OUTPUT() {
#ifdef _CNPUB9
  _CNPUB9 = 0; //disable pullup
#endif
#ifdef _CNPDB9
  _CNPDB9 = 0; //disable pulldown
#endif
#ifdef _ODCB9
  _ODCB9 = 0;
#endif
#ifdef _TRISB9
  _TRISB9 = 0;
#endif
#ifdef _ANSB9
  _ANSB9 = 0;  //disable analog
#endif
}
#ifdef _ODCB9
#define ENABLE_RB9_OPENDRAIN() _ODCB9 = 1
#define DISABLE_RB9_OPENDRAIN() _ODCB9 = 0
#define CONFIG_RB9_AS_DIG_OD_OUTPUT()  {CONFIG_RB9_AS_DIG_OUTPUT();ENABLE_RB9_OPENDRAIN();}
#endif
static inline void CONFIG_RB9_AS_DIG_INPUT() {
#ifdef _CNPUB9
  _CNPUB9 = 0; //disable pullup
#endif
#ifdef _CNPDB9
  _CNPDB9 = 0; //disable pulldown
#endif
#ifdef _TRISB9
  _TRISB9 = 1;
#endif
#ifdef _ANSB9
  _ANSB9 = 0;  //disable analog
#endif
}
#define CONFIG_AN9_AS_DIGITAL() _ANSB9 = 0
#define CONFIG_AN9_AS_ANALOG() _ANSB9 = 1
#endif  //endif #ifdef _RB9
#ifdef _RB10
#ifdef _CNPUB10
#define ENABLE_RB10_PULLUP() _CNPUB10 = 1
#define DISABLE_RB10_PULLUP() _CNPUB10 = 0
#endif
#ifdef _CNPDB10
#define ENABLE_RB10_PULLDOWN() _CNPDB10 = 1
#define DISABLE_RB10_PULLDOWN() _CNPDB10 = 0
#endif
#ifdef _ANSB10
#define ENABLE_RB10_ANALOG() _ANSB10 = 1
#define DISABLE_RB10_ANALOG() _ANSB10 = 0
#endif
#ifdef _CNIEB10
#define ENABLE_RB10_CN_INTERRUPT() _CNIEB10 = 1
#define DISABLE_RB10_CN_INTERRUPT() _CNIEB10 = 0
#endif
static inline void CONFIG_RB10_AS_DIG_OUTPUT() {
#ifdef _CNPUB10
  _CNPUB10 = 0; //disable pullup
#endif
#ifdef _CNPDB10
  _CNPDB10 = 0; //disable pulldown
#endif
#ifdef _ODCB10
  _ODCB10 = 0;
#endif
#ifdef _TRISB10
  _TRISB10 = 0;
#endif
#ifdef _ANSB10
  _ANSB10 = 0;  //disable analog
#endif
}
#ifdef _ODCB10
#define ENABLE_RB10_OPENDRAIN() _ODCB10 = 1
#define DISABLE_RB10_OPENDRAIN() _ODCB10 = 0
#define CONFIG_RB10_AS_DIG_OD_OUTPUT()  {CONFIG_RB10_AS_DIG_OUTPUT();ENABLE_RB10_OPENDRAIN();}
#endif
static inline void CONFIG_RB10_AS_DIG_INPUT() {
#ifdef _CNPUB10
  _CNPUB10 = 0; //disable pullup
#endif
#ifdef _CNPDB10
  _CNPDB10 = 0; //disable pulldown
#endif
#ifdef _TRISB10
  _TRISB10 = 1;
#endif
#ifdef _ANSB10
  _ANSB10 = 0;  //disable analog
#endif
}
#define CONFIG_AN10_AS_DIGITAL() _ANSB10 = 0
#define CONFIG_AN10_AS_ANALOG() _ANSB10 = 1
#endif  //endif #ifdef _RB10
#ifdef _RB11
#ifdef _CNPUB11
#define ENABLE_RB11_PULLUP() _CNPUB11 = 1
#define DISABLE_RB11_PULLUP() _CNPUB11 = 0
#endif
#ifdef _CNPDB11
#define ENABLE_RB11_PULLDOWN() _CNPDB11 = 1
#define DISABLE_RB11_PULLDOWN() _CNPDB11 = 0
#endif
#ifdef _ANSB11
#define ENABLE_RB11_ANALOG() _ANSB11 = 1
#define DISABLE_RB11_ANALOG() _ANSB11 = 0
#endif
#ifdef _CNIEB11
#define ENABLE_RB11_CN_INTERRUPT() _CNIEB11 = 1
#define DISABLE_RB11_CN_INTERRUPT() _CNIEB11 = 0
#endif
static inline void CONFIG_RB11_AS_DIG_OUTPUT() {
#ifdef _CNPUB11
  _CNPUB11 = 0; //disable pullup
#endif
#ifdef _CNPDB11
  _CNPDB11 = 0; //disable pulldown
#endif
#ifdef _ODCB11
  _ODCB11 = 0;
#endif
#ifdef _TRISB11
  _TRISB11 = 0;
#endif
#ifdef _ANSB11
  _ANSB11 = 0;  //disable analog
#endif
}
#ifdef _ODCB11
#define ENABLE_RB11_OPENDRAIN() _ODCB11 = 1
#define DISABLE_RB11_OPENDRAIN() _ODCB11 = 0
#define CONFIG_RB11_AS_DIG_OD_OUTPUT()  {CONFIG_RB11_AS_DIG_OUTPUT();ENABLE_RB11_OPENDRAIN();}
#endif
static inline void CONFIG_RB11_AS_DIG_INPUT() {
#ifdef _CNPUB11
  _CNPUB11 = 0; //disable pullup
#endif
#ifdef _CNPDB11
  _CNPDB11 = 0; //disable pulldown
#endif
#ifdef _TRISB11
  _TRISB11 = 1;
#endif
#ifdef _ANSB11
  _ANSB11 = 0;  //disable analog
#endif
}
#define CONFIG_AN11_AS_DIGITAL() _ANSB11 = 0
#define CONFIG_AN11_AS_ANALOG() _ANSB11 = 1
#endif  //endif #ifdef _RB11
#ifdef _RB12
#ifdef _CNPUB12
#define ENABLE_RB12_PULLUP() _CNPUB12 = 1
#define DISABLE_RB12_PULLUP() _CNPUB12 = 0
#endif
#ifdef _CNPDB12
#define ENABLE_RB12_PULLDOWN() _CNPDB12 = 1
#define DISABLE_RB12_PULLDOWN() _CNPDB12 = 0
#endif
#ifdef _ANSB12
#define ENABLE_RB12_ANALOG() _ANSB12 = 1
#define DISABLE_RB12_ANALOG() _ANSB12 = 0
#endif
#ifdef _CNIEB12
#define ENABLE_RB12_CN_INTERRUPT() _CNIEB12 = 1
#define DISABLE_RB12_CN_INTERRUPT() _CNIEB12 = 0
#endif
static inline void CONFIG_RB12_AS_DIG_OUTPUT() {
#ifdef _CNPUB12
  _CNPUB12 = 0; //disable pullup
#endif
#ifdef _CNPDB12
  _CNPDB12 = 0; //disable pulldown
#endif
#ifdef _ODCB12
  _ODCB12 = 0;
#endif
#ifdef _TRISB12
  _TRISB12 = 0;
#endif
#ifdef _ANSB12
  _ANSB12 = 0;  //disable analog
#endif
}
#ifdef _ODCB12
#define ENABLE_RB12_OPENDRAIN() _ODCB12 = 1
#define DISABLE_RB12_OPENDRAIN() _ODCB12 = 0
#define CONFIG_RB12_AS_DIG_OD_OUTPUT()  {CONFIG_RB12_AS_DIG_OUTPUT();ENABLE_RB12_OPENDRAIN();}
#endif
static inline void CONFIG_RB12_AS_DIG_INPUT() {
#ifdef _CNPUB12
  _CNPUB12 = 0; //disable pullup
#endif
#ifdef _CNPDB12
  _CNPDB12 = 0; //disable pulldown
#endif
#ifdef _TRISB12
  _TRISB12 = 1;
#endif
#ifdef _ANSB12
  _ANSB12 = 0;  //disable analog
#endif
}
#define CONFIG_AN12_AS_DIGITAL() _ANSB12 = 0
#define CONFIG_AN12_AS_ANALOG() _ANSB12 = 1
#endif  //endif #ifdef _RB12
#ifdef _RB13
#ifdef _CNPUB13
#define ENABLE_RB13_PULLUP() _CNPUB13 = 1
#define DISABLE_RB13_PULLUP() _CNPUB13 = 0
#endif
#ifdef _CNPDB13
#define ENABLE_RB13_PULLDOWN() _CNPDB13 = 1
#define DISABLE_RB13_PULLDOWN() _CNPDB13 = 0
#endif
#ifdef _ANSB13
#define ENABLE_RB13_ANALOG() _ANSB13 = 1
#define DISABLE_RB13_ANALOG() _ANSB13 = 0
#endif
#ifdef _CNIEB13
#define ENABLE_RB13_CN_INTERRUPT() _CNIEB13 = 1
#define DISABLE_RB13_CN_INTERRUPT() _CNIEB13 = 0
#endif
static inline void CONFIG_RB13_AS_DIG_OUTPUT() {
#ifdef _CNPUB13
  _CNPUB13 = 0; //disable pullup
#endif
#ifdef _CNPDB13
  _CNPDB13 = 0; //disable pulldown
#endif
#ifdef _ODCB13
  _ODCB13 = 0;
#endif
#ifdef _TRISB13
  _TRISB13 = 0;
#endif
#ifdef _ANSB13
  _ANSB13 = 0;  //disable analog
#endif
}
#ifdef _ODCB13
#define ENABLE_RB13_OPENDRAIN() _ODCB13 = 1
#define DISABLE_RB13_OPENDRAIN() _ODCB13 = 0
#define CONFIG_RB13_AS_DIG_OD_OUTPUT()  {CONFIG_RB13_AS_DIG_OUTPUT();ENABLE_RB13_OPENDRAIN();}
#endif
static inline void CONFIG_RB13_AS_DIG_INPUT() {
#ifdef _CNPUB13
  _CNPUB13 = 0; //disable pullup
#endif
#ifdef _CNPDB13
  _CNPDB13 = 0; //disable pulldown
#endif
#ifdef _TRISB13
  _TRISB13 = 1;
#endif
#ifdef _ANSB13
  _ANSB13 = 0;  //disable analog
#endif
}
#define CONFIG_AN13_AS_DIGITAL() _ANSB13 = 0
#define CONFIG_AN13_AS_ANALOG() _ANSB13 = 1
#endif  //endif #ifdef _RB13
#ifdef _RB14
#ifdef _CNPUB14
#define ENABLE_RB14_PULLUP() _CNPUB14 = 1
#define DISABLE_RB14_PULLUP() _CNPUB14 = 0
#endif
#ifdef _CNPDB14
#define ENABLE_RB14_PULLDOWN() _CNPDB14 = 1
#define DISABLE_RB14_PULLDOWN() _CNPDB14 = 0
#endif
#ifdef _ANSB14
#define ENABLE_RB14_ANALOG() _ANSB14 = 1
#define DISABLE_RB14_ANALOG() _ANSB14 = 0
#endif
#ifdef _CNIEB14
#define ENABLE_RB14_CN_INTERRUPT() _CNIEB14 = 1
#define DISABLE_RB14_CN_INTERRUPT() _CNIEB14 = 0
#endif
static inline void CONFIG_RB14_AS_DIG_OUTPUT() {
#ifdef _CNPUB14
  _CNPUB14 = 0; //disable pullup
#endif
#ifdef _CNPDB14
  _CNPDB14 = 0; //disable pulldown
#endif
#ifdef _ODCB14
  _ODCB14 = 0;
#endif
#ifdef _TRISB14
  _TRISB14 = 0;
#endif
#ifdef _ANSB14
  _ANSB14 = 0;  //disable analog
#endif
}
#ifdef _ODCB14
#define ENABLE_RB14_OPENDRAIN() _ODCB14 = 1
#define DISABLE_RB14_OPENDRAIN() _ODCB14 = 0
#define CONFIG_RB14_AS_DIG_OD_OUTPUT()  {CONFIG_RB14_AS_DIG_OUTPUT();ENABLE_RB14_OPENDRAIN();}
#endif
static inline void CONFIG_RB14_AS_DIG_INPUT() {
#ifdef _CNPUB14
  _CNPUB14 = 0; //disable pullup
#endif
#ifdef _CNPDB14
  _CNPDB14 = 0; //disable pulldown
#endif
#ifdef _TRISB14
  _TRISB14 = 1;
#endif
#ifdef _ANSB14
  _ANSB14 = 0;  //disable analog
#endif
}
#define CONFIG_AN14_AS_DIGITAL() _ANSB14 = 0
#define CONFIG_AN14_AS_ANALOG() _ANSB14 = 1
#endif  //endif #ifdef _RB14
#ifdef _RB15
#ifdef _CNPUB15
#define ENABLE_RB15_PULLUP() _CNPUB15 = 1
#define DISABLE_RB15_PULLUP() _CNPUB15 = 0
#endif
#ifdef _CNPDB15
#define ENABLE_RB15_PULLDOWN() _CNPDB15 = 1
#define DISABLE_RB15_PULLDOWN() _CNPDB15 = 0
#endif
#ifdef _ANSB15
#define ENABLE_RB15_ANALOG() _ANSB15 = 1
#define DISABLE_RB15_ANALOG() _ANSB15 = 0
#endif
#ifdef _CNIEB15
#define ENABLE_RB15_CN_INTERRUPT() _CNIEB15 = 1
#define DISABLE_RB15_CN_INTERRUPT() _CNIEB15 = 0
#endif
static inline void CONFIG_RB15_AS_DIG_OUTPUT() {
#ifdef _CNPUB15
  _CNPUB15 = 0; //disable pullup
#endif
#ifdef _CNPDB15
  _CNPDB15 = 0; //disable pulldown
#endif
#ifdef _ODCB15
  _ODCB15 = 0;
#endif
#ifdef _TRISB15
  _TRISB15 = 0;
#endif
#ifdef _ANSB15
  _ANSB15 = 0;  //disable analog
#endif
}
#ifdef _ODCB15
#define ENABLE_RB15_OPENDRAIN() _ODCB15 = 1
#define DISABLE_RB15_OPENDRAIN() _ODCB15 = 0
#define CONFIG_RB15_AS_DIG_OD_OUTPUT()  {CONFIG_RB15_AS_DIG_OUTPUT();ENABLE_RB15_OPENDRAIN();}
#endif
static inline void CONFIG_RB15_AS_DIG_INPUT() {
#ifdef _CNPUB15
  _CNPUB15 = 0; //disable pullup
#endif
#ifdef _CNPDB15
  _CNPDB15 = 0; //disable pulldown
#endif
#ifdef _TRISB15
  _TRISB15 = 1;
#endif
#ifdef _ANSB15
  _ANSB15 = 0;  //disable analog
#endif
}
#define CONFIG_AN15_AS_DIGITAL() _ANSB15 = 0
#define CONFIG_AN15_AS_ANALOG() _ANSB15 = 1
#endif  //endif #ifdef _RB15
#ifdef _RC0
#ifdef _CNPUC0
#define ENABLE_RC0_PULLUP() _CNPUC0 = 1
#define DISABLE_RC0_PULLUP() _CNPUC0 = 0
#endif
#ifdef _CNPDC0
#define ENABLE_RC0_PULLDOWN() _CNPDC0 = 1
#define DISABLE_RC0_PULLDOWN() _CNPDC0 = 0
#endif
#ifdef _ANSC0
#define ENABLE_RC0_ANALOG() _ANSC0 = 1
#define DISABLE_RC0_ANALOG() _ANSC0 = 0
#endif
#ifdef _CNIEC0
#define ENABLE_RC0_CN_INTERRUPT() _CNIEC0 = 1
#define DISABLE_RC0_CN_INTERRUPT() _CNIEC0 = 0
#endif
static inline void CONFIG_RC0_AS_DIG_OUTPUT() {
#ifdef _CNPUC0
  _CNPUC0 = 0; //disable pullup
#endif
#ifdef _CNPDC0
  _CNPDC0 = 0; //disable pulldown
#endif
#ifdef _ODCC0
  _ODCC0 = 0;
#endif
#ifdef _TRISC0
  _TRISC0 = 0;
#endif
#ifdef _ANSC0
  _ANSC0 = 0;  //disable analog
#endif
}
#ifdef _ODCC0
#define ENABLE_RC0_OPENDRAIN() _ODCC0 = 1
#define DISABLE_RC0_OPENDRAIN() _ODCC0 = 0
#define CONFIG_RC0_AS_DIG_OD_OUTPUT()  {CONFIG_RC0_AS_DIG_OUTPUT();ENABLE_RC0_OPENDRAIN();}
#endif
static inline void CONFIG_RC0_AS_DIG_INPUT() {
#ifdef _CNPUC0
  _CNPUC0 = 0; //disable pullup
#endif
#ifdef _CNPDC0
  _CNPDC0 = 0; //disable pulldown
#endif
#ifdef _TRISC0
  _TRISC0 = 1;
#endif
#ifdef _ANSC0
  _ANSC0 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC0
#ifdef _RC1
#ifdef _CNPUC1
#define ENABLE_RC1_PULLUP() _CNPUC1 = 1
#define DISABLE_RC1_PULLUP() _CNPUC1 = 0
#endif
#ifdef _CNPDC1
#define ENABLE_RC1_PULLDOWN() _CNPDC1 = 1
#define DISABLE_RC1_PULLDOWN() _CNPDC1 = 0
#endif
#ifdef _ANSC1
#define ENABLE_RC1_ANALOG() _ANSC1 = 1
#define DISABLE_RC1_ANALOG() _ANSC1 = 0
#endif
#ifdef _CNIEC1
#define ENABLE_RC1_CN_INTERRUPT() _CNIEC1 = 1
#define DISABLE_RC1_CN_INTERRUPT() _CNIEC1 = 0
#endif
static inline void CONFIG_RC1_AS_DIG_OUTPUT() {
#ifdef _CNPUC1
  _CNPUC1 = 0; //disable pullup
#endif
#ifdef _CNPDC1
  _CNPDC1 = 0; //disable pulldown
#endif
#ifdef _ODCC1
  _ODCC1 = 0;
#endif
#ifdef _TRISC1
  _TRISC1 = 0;
#endif
#ifdef _ANSC1
  _ANSC1 = 0;  //disable analog
#endif
}
#ifdef _ODCC1
#define ENABLE_RC1_OPENDRAIN() _ODCC1 = 1
#define DISABLE_RC1_OPENDRAIN() _ODCC1 = 0
#define CONFIG_RC1_AS_DIG_OD_OUTPUT()  {CONFIG_RC1_AS_DIG_OUTPUT();ENABLE_RC1_OPENDRAIN();}
#endif
static inline void CONFIG_RC1_AS_DIG_INPUT() {
#ifdef _CNPUC1
  _CNPUC1 = 0; //disable pullup
#endif
#ifdef _CNPDC1
  _CNPDC1 = 0; //disable pulldown
#endif
#ifdef _TRISC1
  _TRISC1 = 1;
#endif
#ifdef _ANSC1
  _ANSC1 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC1
#ifdef _RC2
#ifdef _CNPUC2
#define ENABLE_RC2_PULLUP() _CNPUC2 = 1
#define DISABLE_RC2_PULLUP() _CNPUC2 = 0
#endif
#ifdef _CNPDC2
#define ENABLE_RC2_PULLDOWN() _CNPDC2 = 1
#define DISABLE_RC2_PULLDOWN() _CNPDC2 = 0
#endif
#ifdef _ANSC2
#define ENABLE_RC2_ANALOG() _ANSC2 = 1
#define DISABLE_RC2_ANALOG() _ANSC2 = 0
#endif
#ifdef _CNIEC2
#define ENABLE_RC2_CN_INTERRUPT() _CNIEC2 = 1
#define DISABLE_RC2_CN_INTERRUPT() _CNIEC2 = 0
#endif
static inline void CONFIG_RC2_AS_DIG_OUTPUT() {
#ifdef _CNPUC2
  _CNPUC2 = 0; //disable pullup
#endif
#ifdef _CNPDC2
  _CNPDC2 = 0; //disable pulldown
#endif
#ifdef _ODCC2
  _ODCC2 = 0;
#endif
#ifdef _TRISC2
  _TRISC2 = 0;
#endif
#ifdef _ANSC2
  _ANSC2 = 0;  //disable analog
#endif
}
#ifdef _ODCC2
#define ENABLE_RC2_OPENDRAIN() _ODCC2 = 1
#define DISABLE_RC2_OPENDRAIN() _ODCC2 = 0
#define CONFIG_RC2_AS_DIG_OD_OUTPUT()  {CONFIG_RC2_AS_DIG_OUTPUT();ENABLE_RC2_OPENDRAIN();}
#endif
static inline void CONFIG_RC2_AS_DIG_INPUT() {
#ifdef _CNPUC2
  _CNPUC2 = 0; //disable pullup
#endif
#ifdef _CNPDC2
  _CNPDC2 = 0; //disable pulldown
#endif
#ifdef _TRISC2
  _TRISC2 = 1;
#endif
#ifdef _ANSC2
  _ANSC2 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC2
#ifdef _RC3
#ifdef _CNPUC3
#define ENABLE_RC3_PULLUP() _CNPUC3 = 1
#define DISABLE_RC3_PULLUP() _CNPUC3 = 0
#endif
#ifdef _CNPDC3
#define ENABLE_RC3_PULLDOWN() _CNPDC3 = 1
#define DISABLE_RC3_PULLDOWN() _CNPDC3 = 0
#endif
#ifdef _ANSC3
#define ENABLE_RC3_ANALOG() _ANSC3 = 1
#define DISABLE_RC3_ANALOG() _ANSC3 = 0
#endif
#ifdef _CNIEC3
#define ENABLE_RC3_CN_INTERRUPT() _CNIEC3 = 1
#define DISABLE_RC3_CN_INTERRUPT() _CNIEC3 = 0
#endif
static inline void CONFIG_RC3_AS_DIG_OUTPUT() {
#ifdef _CNPUC3
  _CNPUC3 = 0; //disable pullup
#endif
#ifdef _CNPDC3
  _CNPDC3 = 0; //disable pulldown
#endif
#ifdef _ODCC3
  _ODCC3 = 0;
#endif
#ifdef _TRISC3
  _TRISC3 = 0;
#endif
#ifdef _ANSC3
  _ANSC3 = 0;  //disable analog
#endif
}
#ifdef _ODCC3
#define ENABLE_RC3_OPENDRAIN() _ODCC3 = 1
#define DISABLE_RC3_OPENDRAIN() _ODCC3 = 0
#define CONFIG_RC3_AS_DIG_OD_OUTPUT()  {CONFIG_RC3_AS_DIG_OUTPUT();ENABLE_RC3_OPENDRAIN();}
#endif
static inline void CONFIG_RC3_AS_DIG_INPUT() {
#ifdef _CNPUC3
  _CNPUC3 = 0; //disable pullup
#endif
#ifdef _CNPDC3
  _CNPDC3 = 0; //disable pulldown
#endif
#ifdef _TRISC3
  _TRISC3 = 1;
#endif
#ifdef _ANSC3
  _ANSC3 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC3
#ifdef _RC4
#ifdef _CNPUC4
#define ENABLE_RC4_PULLUP() _CNPUC4 = 1
#define DISABLE_RC4_PULLUP() _CNPUC4 = 0
#endif
#ifdef _CNPDC4
#define ENABLE_RC4_PULLDOWN() _CNPDC4 = 1
#define DISABLE_RC4_PULLDOWN() _CNPDC4 = 0
#endif
#ifdef _ANSC4
#define ENABLE_RC4_ANALOG() _ANSC4 = 1
#define DISABLE_RC4_ANALOG() _ANSC4 = 0
#endif
#ifdef _CNIEC4
#define ENABLE_RC4_CN_INTERRUPT() _CNIEC4 = 1
#define DISABLE_RC4_CN_INTERRUPT() _CNIEC4 = 0
#endif
static inline void CONFIG_RC4_AS_DIG_OUTPUT() {
#ifdef _CNPUC4
  _CNPUC4 = 0; //disable pullup
#endif
#ifdef _CNPDC4
  _CNPDC4 = 0; //disable pulldown
#endif
#ifdef _ODCC4
  _ODCC4 = 0;
#endif
#ifdef _TRISC4
  _TRISC4 = 0;
#endif
#ifdef _ANSC4
  _ANSC4 = 0;  //disable analog
#endif
}
#ifdef _ODCC4
#define ENABLE_RC4_OPENDRAIN() _ODCC4 = 1
#define DISABLE_RC4_OPENDRAIN() _ODCC4 = 0
#define CONFIG_RC4_AS_DIG_OD_OUTPUT()  {CONFIG_RC4_AS_DIG_OUTPUT();ENABLE_RC4_OPENDRAIN();}
#endif
static inline void CONFIG_RC4_AS_DIG_INPUT() {
#ifdef _CNPUC4
  _CNPUC4 = 0; //disable pullup
#endif
#ifdef _CNPDC4
  _CNPDC4 = 0; //disable pulldown
#endif
#ifdef _TRISC4
  _TRISC4 = 1;
#endif
#ifdef _ANSC4
  _ANSC4 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC4
#ifdef _RC5
#ifdef _CNPUC5
#define ENABLE_RC5_PULLUP() _CNPUC5 = 1
#define DISABLE_RC5_PULLUP() _CNPUC5 = 0
#endif
#ifdef _CNPDC5
#define ENABLE_RC5_PULLDOWN() _CNPDC5 = 1
#define DISABLE_RC5_PULLDOWN() _CNPDC5 = 0
#endif
#ifdef _ANSC5
#define ENABLE_RC5_ANALOG() _ANSC5 = 1
#define DISABLE_RC5_ANALOG() _ANSC5 = 0
#endif
#ifdef _CNIEC5
#define ENABLE_RC5_CN_INTERRUPT() _CNIEC5 = 1
#define DISABLE_RC5_CN_INTERRUPT() _CNIEC5 = 0
#endif
static inline void CONFIG_RC5_AS_DIG_OUTPUT() {
#ifdef _CNPUC5
  _CNPUC5 = 0; //disable pullup
#endif
#ifdef _CNPDC5
  _CNPDC5 = 0; //disable pulldown
#endif
#ifdef _ODCC5
  _ODCC5 = 0;
#endif
#ifdef _TRISC5
  _TRISC5 = 0;
#endif
#ifdef _ANSC5
  _ANSC5 = 0;  //disable analog
#endif
}
#ifdef _ODCC5
#define ENABLE_RC5_OPENDRAIN() _ODCC5 = 1
#define DISABLE_RC5_OPENDRAIN() _ODCC5 = 0
#define CONFIG_RC5_AS_DIG_OD_OUTPUT()  {CONFIG_RC5_AS_DIG_OUTPUT();ENABLE_RC5_OPENDRAIN();}
#endif
static inline void CONFIG_RC5_AS_DIG_INPUT() {
#ifdef _CNPUC5
  _CNPUC5 = 0; //disable pullup
#endif
#ifdef _CNPDC5
  _CNPDC5 = 0; //disable pulldown
#endif
#ifdef _TRISC5
  _TRISC5 = 1;
#endif
#ifdef _ANSC5
  _ANSC5 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC5
#ifdef _RC6
#ifdef _CNPUC6
#define ENABLE_RC6_PULLUP() _CNPUC6 = 1
#define DISABLE_RC6_PULLUP() _CNPUC6 = 0
#endif
#ifdef _CNPDC6
#define ENABLE_RC6_PULLDOWN() _CNPDC6 = 1
#define DISABLE_RC6_PULLDOWN() _CNPDC6 = 0
#endif
#ifdef _ANSC6
#define ENABLE_RC6_ANALOG() _ANSC6 = 1
#define DISABLE_RC6_ANALOG() _ANSC6 = 0
#endif
#ifdef _CNIEC6
#define ENABLE_RC6_CN_INTERRUPT() _CNIEC6 = 1
#define DISABLE_RC6_CN_INTERRUPT() _CNIEC6 = 0
#endif
static inline void CONFIG_RC6_AS_DIG_OUTPUT() {
#ifdef _CNPUC6
  _CNPUC6 = 0; //disable pullup
#endif
#ifdef _CNPDC6
  _CNPDC6 = 0; //disable pulldown
#endif
#ifdef _ODCC6
  _ODCC6 = 0;
#endif
#ifdef _TRISC6
  _TRISC6 = 0;
#endif
#ifdef _ANSC6
  _ANSC6 = 0;  //disable analog
#endif
}
#ifdef _ODCC6
#define ENABLE_RC6_OPENDRAIN() _ODCC6 = 1
#define DISABLE_RC6_OPENDRAIN() _ODCC6 = 0
#define CONFIG_RC6_AS_DIG_OD_OUTPUT()  {CONFIG_RC6_AS_DIG_OUTPUT();ENABLE_RC6_OPENDRAIN();}
#endif
static inline void CONFIG_RC6_AS_DIG_INPUT() {
#ifdef _CNPUC6
  _CNPUC6 = 0; //disable pullup
#endif
#ifdef _CNPDC6
  _CNPDC6 = 0; //disable pulldown
#endif
#ifdef _TRISC6
  _TRISC6 = 1;
#endif
#ifdef _ANSC6
  _ANSC6 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC6
#ifdef _RC7
#ifdef _CNPUC7
#define ENABLE_RC7_PULLUP() _CNPUC7 = 1
#define DISABLE_RC7_PULLUP() _CNPUC7 = 0
#endif
#ifdef _CNPDC7
#define ENABLE_RC7_PULLDOWN() _CNPDC7 = 1
#define DISABLE_RC7_PULLDOWN() _CNPDC7 = 0
#endif
#ifdef _ANSC7
#define ENABLE_RC7_ANALOG() _ANSC7 = 1
#define DISABLE_RC7_ANALOG() _ANSC7 = 0
#endif
#ifdef _CNIEC7
#define ENABLE_RC7_CN_INTERRUPT() _CNIEC7 = 1
#define DISABLE_RC7_CN_INTERRUPT() _CNIEC7 = 0
#endif
static inline void CONFIG_RC7_AS_DIG_OUTPUT() {
#ifdef _CNPUC7
  _CNPUC7 = 0; //disable pullup
#endif
#ifdef _CNPDC7
  _CNPDC7 = 0; //disable pulldown
#endif
#ifdef _ODCC7
  _ODCC7 = 0;
#endif
#ifdef _TRISC7
  _TRISC7 = 0;
#endif
#ifdef _ANSC7
  _ANSC7 = 0;  //disable analog
#endif
}
#ifdef _ODCC7
#define ENABLE_RC7_OPENDRAIN() _ODCC7 = 1
#define DISABLE_RC7_OPENDRAIN() _ODCC7 = 0
#define CONFIG_RC7_AS_DIG_OD_OUTPUT()  {CONFIG_RC7_AS_DIG_OUTPUT();ENABLE_RC7_OPENDRAIN();}
#endif
static inline void CONFIG_RC7_AS_DIG_INPUT() {
#ifdef _CNPUC7
  _CNPUC7 = 0; //disable pullup
#endif
#ifdef _CNPDC7
  _CNPDC7 = 0; //disable pulldown
#endif
#ifdef _TRISC7
  _TRISC7 = 1;
#endif
#ifdef _ANSC7
  _ANSC7 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC7
#ifdef _RC8
#ifdef _CNPUC8
#define ENABLE_RC8_PULLUP() _CNPUC8 = 1
#define DISABLE_RC8_PULLUP() _CNPUC8 = 0
#endif
#ifdef _CNPDC8
#define ENABLE_RC8_PULLDOWN() _CNPDC8 = 1
#define DISABLE_RC8_PULLDOWN() _CNPDC8 = 0
#endif
#ifdef _ANSC8
#define ENABLE_RC8_ANALOG() _ANSC8 = 1
#define DISABLE_RC8_ANALOG() _ANSC8 = 0
#endif
#ifdef _CNIEC8
#define ENABLE_RC8_CN_INTERRUPT() _CNIEC8 = 1
#define DISABLE_RC8_CN_INTERRUPT() _CNIEC8 = 0
#endif
static inline void CONFIG_RC8_AS_DIG_OUTPUT() {
#ifdef _CNPUC8
  _CNPUC8 = 0; //disable pullup
#endif
#ifdef _CNPDC8
  _CNPDC8 = 0; //disable pulldown
#endif
#ifdef _ODCC8
  _ODCC8 = 0;
#endif
#ifdef _TRISC8
  _TRISC8 = 0;
#endif
#ifdef _ANSC8
  _ANSC8 = 0;  //disable analog
#endif
}
#ifdef _ODCC8
#define ENABLE_RC8_OPENDRAIN() _ODCC8 = 1
#define DISABLE_RC8_OPENDRAIN() _ODCC8 = 0
#define CONFIG_RC8_AS_DIG_OD_OUTPUT()  {CONFIG_RC8_AS_DIG_OUTPUT();ENABLE_RC8_OPENDRAIN();}
#endif
static inline void CONFIG_RC8_AS_DIG_INPUT() {
#ifdef _CNPUC8
  _CNPUC8 = 0; //disable pullup
#endif
#ifdef _CNPDC8
  _CNPDC8 = 0; //disable pulldown
#endif
#ifdef _TRISC8
  _TRISC8 = 1;
#endif
#ifdef _ANSC8
  _ANSC8 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC8
#ifdef _RC9
#ifdef _CNPUC9
#define ENABLE_RC9_PULLUP() _CNPUC9 = 1
#define DISABLE_RC9_PULLUP() _CNPUC9 = 0
#endif
#ifdef _CNPDC9
#define ENABLE_RC9_PULLDOWN() _CNPDC9 = 1
#define DISABLE_RC9_PULLDOWN() _CNPDC9 = 0
#endif
#ifdef _ANSC9
#define ENABLE_RC9_ANALOG() _ANSC9 = 1
#define DISABLE_RC9_ANALOG() _ANSC9 = 0
#endif
#ifdef _CNIEC9
#define ENABLE_RC9_CN_INTERRUPT() _CNIEC9 = 1
#define DISABLE_RC9_CN_INTERRUPT() _CNIEC9 = 0
#endif
static inline void CONFIG_RC9_AS_DIG_OUTPUT() {
#ifdef _CNPUC9
  _CNPUC9 = 0; //disable pullup
#endif
#ifdef _CNPDC9
  _CNPDC9 = 0; //disable pulldown
#endif
#ifdef _ODCC9
  _ODCC9 = 0;
#endif
#ifdef _TRISC9
  _TRISC9 = 0;
#endif
#ifdef _ANSC9
  _ANSC9 = 0;  //disable analog
#endif
}
#ifdef _ODCC9
#define ENABLE_RC9_OPENDRAIN() _ODCC9 = 1
#define DISABLE_RC9_OPENDRAIN() _ODCC9 = 0
#define CONFIG_RC9_AS_DIG_OD_OUTPUT()  {CONFIG_RC9_AS_DIG_OUTPUT();ENABLE_RC9_OPENDRAIN();}
#endif
static inline void CONFIG_RC9_AS_DIG_INPUT() {
#ifdef _CNPUC9
  _CNPUC9 = 0; //disable pullup
#endif
#ifdef _CNPDC9
  _CNPDC9 = 0; //disable pulldown
#endif
#ifdef _TRISC9
  _TRISC9 = 1;
#endif
#ifdef _ANSC9
  _ANSC9 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC9
#ifdef _RC10
#ifdef _CNPUC10
#define ENABLE_RC10_PULLUP() _CNPUC10 = 1
#define DISABLE_RC10_PULLUP() _CNPUC10 = 0
#endif
#ifdef _CNPDC10
#define ENABLE_RC10_PULLDOWN() _CNPDC10 = 1
#define DISABLE_RC10_PULLDOWN() _CNPDC10 = 0
#endif
#ifdef _ANSC10
#define ENABLE_RC10_ANALOG() _ANSC10 = 1
#define DISABLE_RC10_ANALOG() _ANSC10 = 0
#endif
#ifdef _CNIEC10
#define ENABLE_RC10_CN_INTERRUPT() _CNIEC10 = 1
#define DISABLE_RC10_CN_INTERRUPT() _CNIEC10 = 0
#endif
static inline void CONFIG_RC10_AS_DIG_OUTPUT() {
#ifdef _CNPUC10
  _CNPUC10 = 0; //disable pullup
#endif
#ifdef _CNPDC10
  _CNPDC10 = 0; //disable pulldown
#endif
#ifdef _ODCC10
  _ODCC10 = 0;
#endif
#ifdef _TRISC10
  _TRISC10 = 0;
#endif
#ifdef _ANSC10
  _ANSC10 = 0;  //disable analog
#endif
}
#ifdef _ODCC10
#define ENABLE_RC10_OPENDRAIN() _ODCC10 = 1
#define DISABLE_RC10_OPENDRAIN() _ODCC10 = 0
#define CONFIG_RC10_AS_DIG_OD_OUTPUT()  {CONFIG_RC10_AS_DIG_OUTPUT();ENABLE_RC10_OPENDRAIN();}
#endif
static inline void CONFIG_RC10_AS_DIG_INPUT() {
#ifdef _CNPUC10
  _CNPUC10 = 0; //disable pullup
#endif
#ifdef _CNPDC10
  _CNPDC10 = 0; //disable pulldown
#endif
#ifdef _TRISC10
  _TRISC10 = 1;
#endif
#ifdef _ANSC10
  _ANSC10 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC10
#ifdef _RC11
#ifdef _CNPUC11
#define ENABLE_RC11_PULLUP() _CNPUC11 = 1
#define DISABLE_RC11_PULLUP() _CNPUC11 = 0
#endif
#ifdef _CNPDC11
#define ENABLE_RC11_PULLDOWN() _CNPDC11 = 1
#define DISABLE_RC11_PULLDOWN() _CNPDC11 = 0
#endif
#ifdef _ANSC11
#define ENABLE_RC11_ANALOG() _ANSC11 = 1
#define DISABLE_RC11_ANALOG() _ANSC11 = 0
#endif
#ifdef _CNIEC11
#define ENABLE_RC11_CN_INTERRUPT() _CNIEC11 = 1
#define DISABLE_RC11_CN_INTERRUPT() _CNIEC11 = 0
#endif
static inline void CONFIG_RC11_AS_DIG_OUTPUT() {
#ifdef _CNPUC11
  _CNPUC11 = 0; //disable pullup
#endif
#ifdef _CNPDC11
  _CNPDC11 = 0; //disable pulldown
#endif
#ifdef _ODCC11
  _ODCC11 = 0;
#endif
#ifdef _TRISC11
  _TRISC11 = 0;
#endif
#ifdef _ANSC11
  _ANSC11 = 0;  //disable analog
#endif
}
#ifdef _ODCC11
#define ENABLE_RC11_OPENDRAIN() _ODCC11 = 1
#define DISABLE_RC11_OPENDRAIN() _ODCC11 = 0
#define CONFIG_RC11_AS_DIG_OD_OUTPUT()  {CONFIG_RC11_AS_DIG_OUTPUT();ENABLE_RC11_OPENDRAIN();}
#endif
static inline void CONFIG_RC11_AS_DIG_INPUT() {
#ifdef _CNPUC11
  _CNPUC11 = 0; //disable pullup
#endif
#ifdef _CNPDC11
  _CNPDC11 = 0; //disable pulldown
#endif
#ifdef _TRISC11
  _TRISC11 = 1;
#endif
#ifdef _ANSC11
  _ANSC11 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC11
#ifdef _RC12
#ifdef _CNPUC12
#define ENABLE_RC12_PULLUP() _CNPUC12 = 1
#define DISABLE_RC12_PULLUP() _CNPUC12 = 0
#endif
#ifdef _CNPDC12
#define ENABLE_RC12_PULLDOWN() _CNPDC12 = 1
#define DISABLE_RC12_PULLDOWN() _CNPDC12 = 0
#endif
#ifdef _ANSC12
#define ENABLE_RC12_ANALOG() _ANSC12 = 1
#define DISABLE_RC12_ANALOG() _ANSC12 = 0
#endif
#ifdef _CNIEC12
#define ENABLE_RC12_CN_INTERRUPT() _CNIEC12 = 1
#define DISABLE_RC12_CN_INTERRUPT() _CNIEC12 = 0
#endif
static inline void CONFIG_RC12_AS_DIG_OUTPUT() {
#ifdef _CNPUC12
  _CNPUC12 = 0; //disable pullup
#endif
#ifdef _CNPDC12
  _CNPDC12 = 0; //disable pulldown
#endif
#ifdef _ODCC12
  _ODCC12 = 0;
#endif
#ifdef _TRISC12
  _TRISC12 = 0;
#endif
#ifdef _ANSC12
  _ANSC12 = 0;  //disable analog
#endif
}
#ifdef _ODCC12
#define ENABLE_RC12_OPENDRAIN() _ODCC12 = 1
#define DISABLE_RC12_OPENDRAIN() _ODCC12 = 0
#define CONFIG_RC12_AS_DIG_OD_OUTPUT()  {CONFIG_RC12_AS_DIG_OUTPUT();ENABLE_RC12_OPENDRAIN();}
#endif
static inline void CONFIG_RC12_AS_DIG_INPUT() {
#ifdef _CNPUC12
  _CNPUC12 = 0; //disable pullup
#endif
#ifdef _CNPDC12
  _CNPDC12 = 0; //disable pulldown
#endif
#ifdef _TRISC12
  _TRISC12 = 1;
#endif
#ifdef _ANSC12
  _ANSC12 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC12
#ifdef _RC13
#ifdef _CNPUC13
#define ENABLE_RC13_PULLUP() _CNPUC13 = 1
#define DISABLE_RC13_PULLUP() _CNPUC13 = 0
#endif
#ifdef _CNPDC13
#define ENABLE_RC13_PULLDOWN() _CNPDC13 = 1
#define DISABLE_RC13_PULLDOWN() _CNPDC13 = 0
#endif
#ifdef _ANSC13
#define ENABLE_RC13_ANALOG() _ANSC13 = 1
#define DISABLE_RC13_ANALOG() _ANSC13 = 0
#endif
#ifdef _CNIEC13
#define ENABLE_RC13_CN_INTERRUPT() _CNIEC13 = 1
#define DISABLE_RC13_CN_INTERRUPT() _CNIEC13 = 0
#endif
static inline void CONFIG_RC13_AS_DIG_OUTPUT() {
#ifdef _CNPUC13
  _CNPUC13 = 0; //disable pullup
#endif
#ifdef _CNPDC13
  _CNPDC13 = 0; //disable pulldown
#endif
#ifdef _ODCC13
  _ODCC13 = 0;
#endif
#ifdef _TRISC13
  _TRISC13 = 0;
#endif
#ifdef _ANSC13
  _ANSC13 = 0;  //disable analog
#endif
}
#ifdef _ODCC13
#define ENABLE_RC13_OPENDRAIN() _ODCC13 = 1
#define DISABLE_RC13_OPENDRAIN() _ODCC13 = 0
#define CONFIG_RC13_AS_DIG_OD_OUTPUT()  {CONFIG_RC13_AS_DIG_OUTPUT();ENABLE_RC13_OPENDRAIN();}
#endif
static inline void CONFIG_RC13_AS_DIG_INPUT() {
#ifdef _CNPUC13
  _CNPUC13 = 0; //disable pullup
#endif
#ifdef _CNPDC13
  _CNPDC13 = 0; //disable pulldown
#endif
#ifdef _TRISC13
  _TRISC13 = 1;
#endif
#ifdef _ANSC13
  _ANSC13 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC13
#ifdef _RC14
#ifdef _CNPUC14
#define ENABLE_RC14_PULLUP() _CNPUC14 = 1
#define DISABLE_RC14_PULLUP() _CNPUC14 = 0
#endif
#ifdef _CNPDC14
#define ENABLE_RC14_PULLDOWN() _CNPDC14 = 1
#define DISABLE_RC14_PULLDOWN() _CNPDC14 = 0
#endif
#ifdef _ANSC14
#define ENABLE_RC14_ANALOG() _ANSC14 = 1
#define DISABLE_RC14_ANALOG() _ANSC14 = 0
#endif
#ifdef _CNIEC14
#define ENABLE_RC14_CN_INTERRUPT() _CNIEC14 = 1
#define DISABLE_RC14_CN_INTERRUPT() _CNIEC14 = 0
#endif
static inline void CONFIG_RC14_AS_DIG_OUTPUT() {
#ifdef _CNPUC14
  _CNPUC14 = 0; //disable pullup
#endif
#ifdef _CNPDC14
  _CNPDC14 = 0; //disable pulldown
#endif
#ifdef _ODCC14
  _ODCC14 = 0;
#endif
#ifdef _TRISC14
  _TRISC14 = 0;
#endif
#ifdef _ANSC14
  _ANSC14 = 0;  //disable analog
#endif
}
#ifdef _ODCC14
#define ENABLE_RC14_OPENDRAIN() _ODCC14 = 1
#define DISABLE_RC14_OPENDRAIN() _ODCC14 = 0
#define CONFIG_RC14_AS_DIG_OD_OUTPUT()  {CONFIG_RC14_AS_DIG_OUTPUT();ENABLE_RC14_OPENDRAIN();}
#endif
static inline void CONFIG_RC14_AS_DIG_INPUT() {
#ifdef _CNPUC14
  _CNPUC14 = 0; //disable pullup
#endif
#ifdef _CNPDC14
  _CNPDC14 = 0; //disable pulldown
#endif
#ifdef _TRISC14
  _TRISC14 = 1;
#endif
#ifdef _ANSC14
  _ANSC14 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC14
#ifdef _RC15
#ifdef _CNPUC15
#define ENABLE_RC15_PULLUP() _CNPUC15 = 1
#define DISABLE_RC15_PULLUP() _CNPUC15 = 0
#endif
#ifdef _CNPDC15
#define ENABLE_RC15_PULLDOWN() _CNPDC15 = 1
#define DISABLE_RC15_PULLDOWN() _CNPDC15 = 0
#endif
#ifdef _ANSC15
#define ENABLE_RC15_ANALOG() _ANSC15 = 1
#define DISABLE_RC15_ANALOG() _ANSC15 = 0
#endif
#ifdef _CNIEC15
#define ENABLE_RC15_CN_INTERRUPT() _CNIEC15 = 1
#define DISABLE_RC15_CN_INTERRUPT() _CNIEC15 = 0
#endif
static inline void CONFIG_RC15_AS_DIG_OUTPUT() {
#ifdef _CNPUC15
  _CNPUC15 = 0; //disable pullup
#endif
#ifdef _CNPDC15
  _CNPDC15 = 0; //disable pulldown
#endif
#ifdef _ODCC15
  _ODCC15 = 0;
#endif
#ifdef _TRISC15
  _TRISC15 = 0;
#endif
#ifdef _ANSC15
  _ANSC15 = 0;  //disable analog
#endif
}
#ifdef _ODCC15
#define ENABLE_RC15_OPENDRAIN() _ODCC15 = 1
#define DISABLE_RC15_OPENDRAIN() _ODCC15 = 0
#define CONFIG_RC15_AS_DIG_OD_OUTPUT()  {CONFIG_RC15_AS_DIG_OUTPUT();ENABLE_RC15_OPENDRAIN();}
#endif
static inline void CONFIG_RC15_AS_DIG_INPUT() {
#ifdef _CNPUC15
  _CNPUC15 = 0; //disable pullup
#endif
#ifdef _CNPDC15
  _CNPDC15 = 0; //disable pulldown
#endif
#ifdef _TRISC15
  _TRISC15 = 1;
#endif
#ifdef _ANSC15
  _ANSC15 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RC15
#ifdef _RD0
#ifdef _CNPUD0
#define ENABLE_RD0_PULLUP() _CNPUD0 = 1
#define DISABLE_RD0_PULLUP() _CNPUD0 = 0
#endif
#ifdef _CNPDD0
#define ENABLE_RD0_PULLDOWN() _CNPDD0 = 1
#define DISABLE_RD0_PULLDOWN() _CNPDD0 = 0
#endif
#ifdef _ANSD0
#define ENABLE_RD0_ANALOG() _ANSD0 = 1
#define DISABLE_RD0_ANALOG() _ANSD0 = 0
#endif
#ifdef _CNIED0
#define ENABLE_RD0_CN_INTERRUPT() _CNIED0 = 1
#define DISABLE_RD0_CN_INTERRUPT() _CNIED0 = 0
#endif
static inline void CONFIG_RD0_AS_DIG_OUTPUT() {
#ifdef _CNPUD0
  _CNPUD0 = 0; //disable pullup
#endif
#ifdef _CNPDD0
  _CNPDD0 = 0; //disable pulldown
#endif
#ifdef _ODCD0
  _ODCD0 = 0;
#endif
#ifdef _TRISD0
  _TRISD0 = 0;
#endif
#ifdef _ANSD0
  _ANSD0 = 0;  //disable analog
#endif
}
#ifdef _ODCD0
#define ENABLE_RD0_OPENDRAIN() _ODCD0 = 1
#define DISABLE_RD0_OPENDRAIN() _ODCD0 = 0
#define CONFIG_RD0_AS_DIG_OD_OUTPUT()  {CONFIG_RD0_AS_DIG_OUTPUT();ENABLE_RD0_OPENDRAIN();}
#endif
static inline void CONFIG_RD0_AS_DIG_INPUT() {
#ifdef _CNPUD0
  _CNPUD0 = 0; //disable pullup
#endif
#ifdef _CNPDD0
  _CNPDD0 = 0; //disable pulldown
#endif
#ifdef _TRISD0
  _TRISD0 = 1;
#endif
#ifdef _ANSD0
  _ANSD0 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD0
#ifdef _RD1
#ifdef _CNPUD1
#define ENABLE_RD1_PULLUP() _CNPUD1 = 1
#define DISABLE_RD1_PULLUP() _CNPUD1 = 0
#endif
#ifdef _CNPDD1
#define ENABLE_RD1_PULLDOWN() _CNPDD1 = 1
#define DISABLE_RD1_PULLDOWN() _CNPDD1 = 0
#endif
#ifdef _ANSD1
#define ENABLE_RD1_ANALOG() _ANSD1 = 1
#define DISABLE_RD1_ANALOG() _ANSD1 = 0
#endif
#ifdef _CNIED1
#define ENABLE_RD1_CN_INTERRUPT() _CNIED1 = 1
#define DISABLE_RD1_CN_INTERRUPT() _CNIED1 = 0
#endif
static inline void CONFIG_RD1_AS_DIG_OUTPUT() {
#ifdef _CNPUD1
  _CNPUD1 = 0; //disable pullup
#endif
#ifdef _CNPDD1
  _CNPDD1 = 0; //disable pulldown
#endif
#ifdef _ODCD1
  _ODCD1 = 0;
#endif
#ifdef _TRISD1
  _TRISD1 = 0;
#endif
#ifdef _ANSD1
  _ANSD1 = 0;  //disable analog
#endif
}
#ifdef _ODCD1
#define ENABLE_RD1_OPENDRAIN() _ODCD1 = 1
#define DISABLE_RD1_OPENDRAIN() _ODCD1 = 0
#define CONFIG_RD1_AS_DIG_OD_OUTPUT()  {CONFIG_RD1_AS_DIG_OUTPUT();ENABLE_RD1_OPENDRAIN();}
#endif
static inline void CONFIG_RD1_AS_DIG_INPUT() {
#ifdef _CNPUD1
  _CNPUD1 = 0; //disable pullup
#endif
#ifdef _CNPDD1
  _CNPDD1 = 0; //disable pulldown
#endif
#ifdef _TRISD1
  _TRISD1 = 1;
#endif
#ifdef _ANSD1
  _ANSD1 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD1
#ifdef _RD2
#ifdef _CNPUD2
#define ENABLE_RD2_PULLUP() _CNPUD2 = 1
#define DISABLE_RD2_PULLUP() _CNPUD2 = 0
#endif
#ifdef _CNPDD2
#define ENABLE_RD2_PULLDOWN() _CNPDD2 = 1
#define DISABLE_RD2_PULLDOWN() _CNPDD2 = 0
#endif
#ifdef _ANSD2
#define ENABLE_RD2_ANALOG() _ANSD2 = 1
#define DISABLE_RD2_ANALOG() _ANSD2 = 0
#endif
#ifdef _CNIED2
#define ENABLE_RD2_CN_INTERRUPT() _CNIED2 = 1
#define DISABLE_RD2_CN_INTERRUPT() _CNIED2 = 0
#endif
static inline void CONFIG_RD2_AS_DIG_OUTPUT() {
#ifdef _CNPUD2
  _CNPUD2 = 0; //disable pullup
#endif
#ifdef _CNPDD2
  _CNPDD2 = 0; //disable pulldown
#endif
#ifdef _ODCD2
  _ODCD2 = 0;
#endif
#ifdef _TRISD2
  _TRISD2 = 0;
#endif
#ifdef _ANSD2
  _ANSD2 = 0;  //disable analog
#endif
}
#ifdef _ODCD2
#define ENABLE_RD2_OPENDRAIN() _ODCD2 = 1
#define DISABLE_RD2_OPENDRAIN() _ODCD2 = 0
#define CONFIG_RD2_AS_DIG_OD_OUTPUT()  {CONFIG_RD2_AS_DIG_OUTPUT();ENABLE_RD2_OPENDRAIN();}
#endif
static inline void CONFIG_RD2_AS_DIG_INPUT() {
#ifdef _CNPUD2
  _CNPUD2 = 0; //disable pullup
#endif
#ifdef _CNPDD2
  _CNPDD2 = 0; //disable pulldown
#endif
#ifdef _TRISD2
  _TRISD2 = 1;
#endif
#ifdef _ANSD2
  _ANSD2 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD2
#ifdef _RD3
#ifdef _CNPUD3
#define ENABLE_RD3_PULLUP() _CNPUD3 = 1
#define DISABLE_RD3_PULLUP() _CNPUD3 = 0
#endif
#ifdef _CNPDD3
#define ENABLE_RD3_PULLDOWN() _CNPDD3 = 1
#define DISABLE_RD3_PULLDOWN() _CNPDD3 = 0
#endif
#ifdef _ANSD3
#define ENABLE_RD3_ANALOG() _ANSD3 = 1
#define DISABLE_RD3_ANALOG() _ANSD3 = 0
#endif
#ifdef _CNIED3
#define ENABLE_RD3_CN_INTERRUPT() _CNIED3 = 1
#define DISABLE_RD3_CN_INTERRUPT() _CNIED3 = 0
#endif
static inline void CONFIG_RD3_AS_DIG_OUTPUT() {
#ifdef _CNPUD3
  _CNPUD3 = 0; //disable pullup
#endif
#ifdef _CNPDD3
  _CNPDD3 = 0; //disable pulldown
#endif
#ifdef _ODCD3
  _ODCD3 = 0;
#endif
#ifdef _TRISD3
  _TRISD3 = 0;
#endif
#ifdef _ANSD3
  _ANSD3 = 0;  //disable analog
#endif
}
#ifdef _ODCD3
#define ENABLE_RD3_OPENDRAIN() _ODCD3 = 1
#define DISABLE_RD3_OPENDRAIN() _ODCD3 = 0
#define CONFIG_RD3_AS_DIG_OD_OUTPUT()  {CONFIG_RD3_AS_DIG_OUTPUT();ENABLE_RD3_OPENDRAIN();}
#endif
static inline void CONFIG_RD3_AS_DIG_INPUT() {
#ifdef _CNPUD3
  _CNPUD3 = 0; //disable pullup
#endif
#ifdef _CNPDD3
  _CNPDD3 = 0; //disable pulldown
#endif
#ifdef _TRISD3
  _TRISD3 = 1;
#endif
#ifdef _ANSD3
  _ANSD3 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD3
#ifdef _RD4
#ifdef _CNPUD4
#define ENABLE_RD4_PULLUP() _CNPUD4 = 1
#define DISABLE_RD4_PULLUP() _CNPUD4 = 0
#endif
#ifdef _CNPDD4
#define ENABLE_RD4_PULLDOWN() _CNPDD4 = 1
#define DISABLE_RD4_PULLDOWN() _CNPDD4 = 0
#endif
#ifdef _ANSD4
#define ENABLE_RD4_ANALOG() _ANSD4 = 1
#define DISABLE_RD4_ANALOG() _ANSD4 = 0
#endif
#ifdef _CNIED4
#define ENABLE_RD4_CN_INTERRUPT() _CNIED4 = 1
#define DISABLE_RD4_CN_INTERRUPT() _CNIED4 = 0
#endif
static inline void CONFIG_RD4_AS_DIG_OUTPUT() {
#ifdef _CNPUD4
  _CNPUD4 = 0; //disable pullup
#endif
#ifdef _CNPDD4
  _CNPDD4 = 0; //disable pulldown
#endif
#ifdef _ODCD4
  _ODCD4 = 0;
#endif
#ifdef _TRISD4
  _TRISD4 = 0;
#endif
#ifdef _ANSD4
  _ANSD4 = 0;  //disable analog
#endif
}
#ifdef _ODCD4
#define ENABLE_RD4_OPENDRAIN() _ODCD4 = 1
#define DISABLE_RD4_OPENDRAIN() _ODCD4 = 0
#define CONFIG_RD4_AS_DIG_OD_OUTPUT()  {CONFIG_RD4_AS_DIG_OUTPUT();ENABLE_RD4_OPENDRAIN();}
#endif
static inline void CONFIG_RD4_AS_DIG_INPUT() {
#ifdef _CNPUD4
  _CNPUD4 = 0; //disable pullup
#endif
#ifdef _CNPDD4
  _CNPDD4 = 0; //disable pulldown
#endif
#ifdef _TRISD4
  _TRISD4 = 1;
#endif
#ifdef _ANSD4
  _ANSD4 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD4
#ifdef _RD5
#ifdef _CNPUD5
#define ENABLE_RD5_PULLUP() _CNPUD5 = 1
#define DISABLE_RD5_PULLUP() _CNPUD5 = 0
#endif
#ifdef _CNPDD5
#define ENABLE_RD5_PULLDOWN() _CNPDD5 = 1
#define DISABLE_RD5_PULLDOWN() _CNPDD5 = 0
#endif
#ifdef _ANSD5
#define ENABLE_RD5_ANALOG() _ANSD5 = 1
#define DISABLE_RD5_ANALOG() _ANSD5 = 0
#endif
#ifdef _CNIED5
#define ENABLE_RD5_CN_INTERRUPT() _CNIED5 = 1
#define DISABLE_RD5_CN_INTERRUPT() _CNIED5 = 0
#endif
static inline void CONFIG_RD5_AS_DIG_OUTPUT() {
#ifdef _CNPUD5
  _CNPUD5 = 0; //disable pullup
#endif
#ifdef _CNPDD5
  _CNPDD5 = 0; //disable pulldown
#endif
#ifdef _ODCD5
  _ODCD5 = 0;
#endif
#ifdef _TRISD5
  _TRISD5 = 0;
#endif
#ifdef _ANSD5
  _ANSD5 = 0;  //disable analog
#endif
}
#ifdef _ODCD5
#define ENABLE_RD5_OPENDRAIN() _ODCD5 = 1
#define DISABLE_RD5_OPENDRAIN() _ODCD5 = 0
#define CONFIG_RD5_AS_DIG_OD_OUTPUT()  {CONFIG_RD5_AS_DIG_OUTPUT();ENABLE_RD5_OPENDRAIN();}
#endif
static inline void CONFIG_RD5_AS_DIG_INPUT() {
#ifdef _CNPUD5
  _CNPUD5 = 0; //disable pullup
#endif
#ifdef _CNPDD5
  _CNPDD5 = 0; //disable pulldown
#endif
#ifdef _TRISD5
  _TRISD5 = 1;
#endif
#ifdef _ANSD5
  _ANSD5 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD5
#ifdef _RD6
#ifdef _CNPUD6
#define ENABLE_RD6_PULLUP() _CNPUD6 = 1
#define DISABLE_RD6_PULLUP() _CNPUD6 = 0
#endif
#ifdef _CNPDD6
#define ENABLE_RD6_PULLDOWN() _CNPDD6 = 1
#define DISABLE_RD6_PULLDOWN() _CNPDD6 = 0
#endif
#ifdef _ANSD6
#define ENABLE_RD6_ANALOG() _ANSD6 = 1
#define DISABLE_RD6_ANALOG() _ANSD6 = 0
#endif
#ifdef _CNIED6
#define ENABLE_RD6_CN_INTERRUPT() _CNIED6 = 1
#define DISABLE_RD6_CN_INTERRUPT() _CNIED6 = 0
#endif
static inline void CONFIG_RD6_AS_DIG_OUTPUT() {
#ifdef _CNPUD6
  _CNPUD6 = 0; //disable pullup
#endif
#ifdef _CNPDD6
  _CNPDD6 = 0; //disable pulldown
#endif
#ifdef _ODCD6
  _ODCD6 = 0;
#endif
#ifdef _TRISD6
  _TRISD6 = 0;
#endif
#ifdef _ANSD6
  _ANSD6 = 0;  //disable analog
#endif
}
#ifdef _ODCD6
#define ENABLE_RD6_OPENDRAIN() _ODCD6 = 1
#define DISABLE_RD6_OPENDRAIN() _ODCD6 = 0
#define CONFIG_RD6_AS_DIG_OD_OUTPUT()  {CONFIG_RD6_AS_DIG_OUTPUT();ENABLE_RD6_OPENDRAIN();}
#endif
static inline void CONFIG_RD6_AS_DIG_INPUT() {
#ifdef _CNPUD6
  _CNPUD6 = 0; //disable pullup
#endif
#ifdef _CNPDD6
  _CNPDD6 = 0; //disable pulldown
#endif
#ifdef _TRISD6
  _TRISD6 = 1;
#endif
#ifdef _ANSD6
  _ANSD6 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD6
#ifdef _RD7
#ifdef _CNPUD7
#define ENABLE_RD7_PULLUP() _CNPUD7 = 1
#define DISABLE_RD7_PULLUP() _CNPUD7 = 0
#endif
#ifdef _CNPDD7
#define ENABLE_RD7_PULLDOWN() _CNPDD7 = 1
#define DISABLE_RD7_PULLDOWN() _CNPDD7 = 0
#endif
#ifdef _ANSD7
#define ENABLE_RD7_ANALOG() _ANSD7 = 1
#define DISABLE_RD7_ANALOG() _ANSD7 = 0
#endif
#ifdef _CNIED7
#define ENABLE_RD7_CN_INTERRUPT() _CNIED7 = 1
#define DISABLE_RD7_CN_INTERRUPT() _CNIED7 = 0
#endif
static inline void CONFIG_RD7_AS_DIG_OUTPUT() {
#ifdef _CNPUD7
  _CNPUD7 = 0; //disable pullup
#endif
#ifdef _CNPDD7
  _CNPDD7 = 0; //disable pulldown
#endif
#ifdef _ODCD7
  _ODCD7 = 0;
#endif
#ifdef _TRISD7
  _TRISD7 = 0;
#endif
#ifdef _ANSD7
  _ANSD7 = 0;  //disable analog
#endif
}
#ifdef _ODCD7
#define ENABLE_RD7_OPENDRAIN() _ODCD7 = 1
#define DISABLE_RD7_OPENDRAIN() _ODCD7 = 0
#define CONFIG_RD7_AS_DIG_OD_OUTPUT()  {CONFIG_RD7_AS_DIG_OUTPUT();ENABLE_RD7_OPENDRAIN();}
#endif
static inline void CONFIG_RD7_AS_DIG_INPUT() {
#ifdef _CNPUD7
  _CNPUD7 = 0; //disable pullup
#endif
#ifdef _CNPDD7
  _CNPDD7 = 0; //disable pulldown
#endif
#ifdef _TRISD7
  _TRISD7 = 1;
#endif
#ifdef _ANSD7
  _ANSD7 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD7
#ifdef _RD8
#ifdef _CNPUD8
#define ENABLE_RD8_PULLUP() _CNPUD8 = 1
#define DISABLE_RD8_PULLUP() _CNPUD8 = 0
#endif
#ifdef _CNPDD8
#define ENABLE_RD8_PULLDOWN() _CNPDD8 = 1
#define DISABLE_RD8_PULLDOWN() _CNPDD8 = 0
#endif
#ifdef _ANSD8
#define ENABLE_RD8_ANALOG() _ANSD8 = 1
#define DISABLE_RD8_ANALOG() _ANSD8 = 0
#endif
#ifdef _CNIED8
#define ENABLE_RD8_CN_INTERRUPT() _CNIED8 = 1
#define DISABLE_RD8_CN_INTERRUPT() _CNIED8 = 0
#endif
static inline void CONFIG_RD8_AS_DIG_OUTPUT() {
#ifdef _CNPUD8
  _CNPUD8 = 0; //disable pullup
#endif
#ifdef _CNPDD8
  _CNPDD8 = 0; //disable pulldown
#endif
#ifdef _ODCD8
  _ODCD8 = 0;
#endif
#ifdef _TRISD8
  _TRISD8 = 0;
#endif
#ifdef _ANSD8
  _ANSD8 = 0;  //disable analog
#endif
}
#ifdef _ODCD8
#define ENABLE_RD8_OPENDRAIN() _ODCD8 = 1
#define DISABLE_RD8_OPENDRAIN() _ODCD8 = 0
#define CONFIG_RD8_AS_DIG_OD_OUTPUT()  {CONFIG_RD8_AS_DIG_OUTPUT();ENABLE_RD8_OPENDRAIN();}
#endif
static inline void CONFIG_RD8_AS_DIG_INPUT() {
#ifdef _CNPUD8
  _CNPUD8 = 0; //disable pullup
#endif
#ifdef _CNPDD8
  _CNPDD8 = 0; //disable pulldown
#endif
#ifdef _TRISD8
  _TRISD8 = 1;
#endif
#ifdef _ANSD8
  _ANSD8 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD8
#ifdef _RD9
#ifdef _CNPUD9
#define ENABLE_RD9_PULLUP() _CNPUD9 = 1
#define DISABLE_RD9_PULLUP() _CNPUD9 = 0
#endif
#ifdef _CNPDD9
#define ENABLE_RD9_PULLDOWN() _CNPDD9 = 1
#define DISABLE_RD9_PULLDOWN() _CNPDD9 = 0
#endif
#ifdef _ANSD9
#define ENABLE_RD9_ANALOG() _ANSD9 = 1
#define DISABLE_RD9_ANALOG() _ANSD9 = 0
#endif
#ifdef _CNIED9
#define ENABLE_RD9_CN_INTERRUPT() _CNIED9 = 1
#define DISABLE_RD9_CN_INTERRUPT() _CNIED9 = 0
#endif
static inline void CONFIG_RD9_AS_DIG_OUTPUT() {
#ifdef _CNPUD9
  _CNPUD9 = 0; //disable pullup
#endif
#ifdef _CNPDD9
  _CNPDD9 = 0; //disable pulldown
#endif
#ifdef _ODCD9
  _ODCD9 = 0;
#endif
#ifdef _TRISD9
  _TRISD9 = 0;
#endif
#ifdef _ANSD9
  _ANSD9 = 0;  //disable analog
#endif
}
#ifdef _ODCD9
#define ENABLE_RD9_OPENDRAIN() _ODCD9 = 1
#define DISABLE_RD9_OPENDRAIN() _ODCD9 = 0
#define CONFIG_RD9_AS_DIG_OD_OUTPUT()  {CONFIG_RD9_AS_DIG_OUTPUT();ENABLE_RD9_OPENDRAIN();}
#endif
static inline void CONFIG_RD9_AS_DIG_INPUT() {
#ifdef _CNPUD9
  _CNPUD9 = 0; //disable pullup
#endif
#ifdef _CNPDD9
  _CNPDD9 = 0; //disable pulldown
#endif
#ifdef _TRISD9
  _TRISD9 = 1;
#endif
#ifdef _ANSD9
  _ANSD9 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD9
#ifdef _RD10
#ifdef _CNPUD10
#define ENABLE_RD10_PULLUP() _CNPUD10 = 1
#define DISABLE_RD10_PULLUP() _CNPUD10 = 0
#endif
#ifdef _CNPDD10
#define ENABLE_RD10_PULLDOWN() _CNPDD10 = 1
#define DISABLE_RD10_PULLDOWN() _CNPDD10 = 0
#endif
#ifdef _ANSD10
#define ENABLE_RD10_ANALOG() _ANSD10 = 1
#define DISABLE_RD10_ANALOG() _ANSD10 = 0
#endif
#ifdef _CNIED10
#define ENABLE_RD10_CN_INTERRUPT() _CNIED10 = 1
#define DISABLE_RD10_CN_INTERRUPT() _CNIED10 = 0
#endif
static inline void CONFIG_RD10_AS_DIG_OUTPUT() {
#ifdef _CNPUD10
  _CNPUD10 = 0; //disable pullup
#endif
#ifdef _CNPDD10
  _CNPDD10 = 0; //disable pulldown
#endif
#ifdef _ODCD10
  _ODCD10 = 0;
#endif
#ifdef _TRISD10
  _TRISD10 = 0;
#endif
#ifdef _ANSD10
  _ANSD10 = 0;  //disable analog
#endif
}
#ifdef _ODCD10
#define ENABLE_RD10_OPENDRAIN() _ODCD10 = 1
#define DISABLE_RD10_OPENDRAIN() _ODCD10 = 0
#define CONFIG_RD10_AS_DIG_OD_OUTPUT()  {CONFIG_RD10_AS_DIG_OUTPUT();ENABLE_RD10_OPENDRAIN();}
#endif
static inline void CONFIG_RD10_AS_DIG_INPUT() {
#ifdef _CNPUD10
  _CNPUD10 = 0; //disable pullup
#endif
#ifdef _CNPDD10
  _CNPDD10 = 0; //disable pulldown
#endif
#ifdef _TRISD10
  _TRISD10 = 1;
#endif
#ifdef _ANSD10
  _ANSD10 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD10
#ifdef _RD11
#ifdef _CNPUD11
#define ENABLE_RD11_PULLUP() _CNPUD11 = 1
#define DISABLE_RD11_PULLUP() _CNPUD11 = 0
#endif
#ifdef _CNPDD11
#define ENABLE_RD11_PULLDOWN() _CNPDD11 = 1
#define DISABLE_RD11_PULLDOWN() _CNPDD11 = 0
#endif
#ifdef _ANSD11
#define ENABLE_RD11_ANALOG() _ANSD11 = 1
#define DISABLE_RD11_ANALOG() _ANSD11 = 0
#endif
#ifdef _CNIED11
#define ENABLE_RD11_CN_INTERRUPT() _CNIED11 = 1
#define DISABLE_RD11_CN_INTERRUPT() _CNIED11 = 0
#endif
static inline void CONFIG_RD11_AS_DIG_OUTPUT() {
#ifdef _CNPUD11
  _CNPUD11 = 0; //disable pullup
#endif
#ifdef _CNPDD11
  _CNPDD11 = 0; //disable pulldown
#endif
#ifdef _ODCD11
  _ODCD11 = 0;
#endif
#ifdef _TRISD11
  _TRISD11 = 0;
#endif
#ifdef _ANSD11
  _ANSD11 = 0;  //disable analog
#endif
}
#ifdef _ODCD11
#define ENABLE_RD11_OPENDRAIN() _ODCD11 = 1
#define DISABLE_RD11_OPENDRAIN() _ODCD11 = 0
#define CONFIG_RD11_AS_DIG_OD_OUTPUT()  {CONFIG_RD11_AS_DIG_OUTPUT();ENABLE_RD11_OPENDRAIN();}
#endif
static inline void CONFIG_RD11_AS_DIG_INPUT() {
#ifdef _CNPUD11
  _CNPUD11 = 0; //disable pullup
#endif
#ifdef _CNPDD11
  _CNPDD11 = 0; //disable pulldown
#endif
#ifdef _TRISD11
  _TRISD11 = 1;
#endif
#ifdef _ANSD11
  _ANSD11 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD11
#ifdef _RD12
#ifdef _CNPUD12
#define ENABLE_RD12_PULLUP() _CNPUD12 = 1
#define DISABLE_RD12_PULLUP() _CNPUD12 = 0
#endif
#ifdef _CNPDD12
#define ENABLE_RD12_PULLDOWN() _CNPDD12 = 1
#define DISABLE_RD12_PULLDOWN() _CNPDD12 = 0
#endif
#ifdef _ANSD12
#define ENABLE_RD12_ANALOG() _ANSD12 = 1
#define DISABLE_RD12_ANALOG() _ANSD12 = 0
#endif
#ifdef _CNIED12
#define ENABLE_RD12_CN_INTERRUPT() _CNIED12 = 1
#define DISABLE_RD12_CN_INTERRUPT() _CNIED12 = 0
#endif
static inline void CONFIG_RD12_AS_DIG_OUTPUT() {
#ifdef _CNPUD12
  _CNPUD12 = 0; //disable pullup
#endif
#ifdef _CNPDD12
  _CNPDD12 = 0; //disable pulldown
#endif
#ifdef _ODCD12
  _ODCD12 = 0;
#endif
#ifdef _TRISD12
  _TRISD12 = 0;
#endif
#ifdef _ANSD12
  _ANSD12 = 0;  //disable analog
#endif
}
#ifdef _ODCD12
#define ENABLE_RD12_OPENDRAIN() _ODCD12 = 1
#define DISABLE_RD12_OPENDRAIN() _ODCD12 = 0
#define CONFIG_RD12_AS_DIG_OD_OUTPUT()  {CONFIG_RD12_AS_DIG_OUTPUT();ENABLE_RD12_OPENDRAIN();}
#endif
static inline void CONFIG_RD12_AS_DIG_INPUT() {
#ifdef _CNPUD12
  _CNPUD12 = 0; //disable pullup
#endif
#ifdef _CNPDD12
  _CNPDD12 = 0; //disable pulldown
#endif
#ifdef _TRISD12
  _TRISD12 = 1;
#endif
#ifdef _ANSD12
  _ANSD12 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD12
#ifdef _RD13
#ifdef _CNPUD13
#define ENABLE_RD13_PULLUP() _CNPUD13 = 1
#define DISABLE_RD13_PULLUP() _CNPUD13 = 0
#endif
#ifdef _CNPDD13
#define ENABLE_RD13_PULLDOWN() _CNPDD13 = 1
#define DISABLE_RD13_PULLDOWN() _CNPDD13 = 0
#endif
#ifdef _ANSD13
#define ENABLE_RD13_ANALOG() _ANSD13 = 1
#define DISABLE_RD13_ANALOG() _ANSD13 = 0
#endif
#ifdef _CNIED13
#define ENABLE_RD13_CN_INTERRUPT() _CNIED13 = 1
#define DISABLE_RD13_CN_INTERRUPT() _CNIED13 = 0
#endif
static inline void CONFIG_RD13_AS_DIG_OUTPUT() {
#ifdef _CNPUD13
  _CNPUD13 = 0; //disable pullup
#endif
#ifdef _CNPDD13
  _CNPDD13 = 0; //disable pulldown
#endif
#ifdef _ODCD13
  _ODCD13 = 0;
#endif
#ifdef _TRISD13
  _TRISD13 = 0;
#endif
#ifdef _ANSD13
  _ANSD13 = 0;  //disable analog
#endif
}
#ifdef _ODCD13
#define ENABLE_RD13_OPENDRAIN() _ODCD13 = 1
#define DISABLE_RD13_OPENDRAIN() _ODCD13 = 0
#define CONFIG_RD13_AS_DIG_OD_OUTPUT()  {CONFIG_RD13_AS_DIG_OUTPUT();ENABLE_RD13_OPENDRAIN();}
#endif
static inline void CONFIG_RD13_AS_DIG_INPUT() {
#ifdef _CNPUD13
  _CNPUD13 = 0; //disable pullup
#endif
#ifdef _CNPDD13
  _CNPDD13 = 0; //disable pulldown
#endif
#ifdef _TRISD13
  _TRISD13 = 1;
#endif
#ifdef _ANSD13
  _ANSD13 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD13
#ifdef _RD14
#ifdef _CNPUD14
#define ENABLE_RD14_PULLUP() _CNPUD14 = 1
#define DISABLE_RD14_PULLUP() _CNPUD14 = 0
#endif
#ifdef _CNPDD14
#define ENABLE_RD14_PULLDOWN() _CNPDD14 = 1
#define DISABLE_RD14_PULLDOWN() _CNPDD14 = 0
#endif
#ifdef _ANSD14
#define ENABLE_RD14_ANALOG() _ANSD14 = 1
#define DISABLE_RD14_ANALOG() _ANSD14 = 0
#endif
#ifdef _CNIED14
#define ENABLE_RD14_CN_INTERRUPT() _CNIED14 = 1
#define DISABLE_RD14_CN_INTERRUPT() _CNIED14 = 0
#endif
static inline void CONFIG_RD14_AS_DIG_OUTPUT() {
#ifdef _CNPUD14
  _CNPUD14 = 0; //disable pullup
#endif
#ifdef _CNPDD14
  _CNPDD14 = 0; //disable pulldown
#endif
#ifdef _ODCD14
  _ODCD14 = 0;
#endif
#ifdef _TRISD14
  _TRISD14 = 0;
#endif
#ifdef _ANSD14
  _ANSD14 = 0;  //disable analog
#endif
}
#ifdef _ODCD14
#define ENABLE_RD14_OPENDRAIN() _ODCD14 = 1
#define DISABLE_RD14_OPENDRAIN() _ODCD14 = 0
#define CONFIG_RD14_AS_DIG_OD_OUTPUT()  {CONFIG_RD14_AS_DIG_OUTPUT();ENABLE_RD14_OPENDRAIN();}
#endif
static inline void CONFIG_RD14_AS_DIG_INPUT() {
#ifdef _CNPUD14
  _CNPUD14 = 0; //disable pullup
#endif
#ifdef _CNPDD14
  _CNPDD14 = 0; //disable pulldown
#endif
#ifdef _TRISD14
  _TRISD14 = 1;
#endif
#ifdef _ANSD14
  _ANSD14 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD14
#ifdef _RD15
#ifdef _CNPUD15
#define ENABLE_RD15_PULLUP() _CNPUD15 = 1
#define DISABLE_RD15_PULLUP() _CNPUD15 = 0
#endif
#ifdef _CNPDD15
#define ENABLE_RD15_PULLDOWN() _CNPDD15 = 1
#define DISABLE_RD15_PULLDOWN() _CNPDD15 = 0
#endif
#ifdef _ANSD15
#define ENABLE_RD15_ANALOG() _ANSD15 = 1
#define DISABLE_RD15_ANALOG() _ANSD15 = 0
#endif
#ifdef _CNIED15
#define ENABLE_RD15_CN_INTERRUPT() _CNIED15 = 1
#define DISABLE_RD15_CN_INTERRUPT() _CNIED15 = 0
#endif
static inline void CONFIG_RD15_AS_DIG_OUTPUT() {
#ifdef _CNPUD15
  _CNPUD15 = 0; //disable pullup
#endif
#ifdef _CNPDD15
  _CNPDD15 = 0; //disable pulldown
#endif
#ifdef _ODCD15
  _ODCD15 = 0;
#endif
#ifdef _TRISD15
  _TRISD15 = 0;
#endif
#ifdef _ANSD15
  _ANSD15 = 0;  //disable analog
#endif
}
#ifdef _ODCD15
#define ENABLE_RD15_OPENDRAIN() _ODCD15 = 1
#define DISABLE_RD15_OPENDRAIN() _ODCD15 = 0
#define CONFIG_RD15_AS_DIG_OD_OUTPUT()  {CONFIG_RD15_AS_DIG_OUTPUT();ENABLE_RD15_OPENDRAIN();}
#endif
static inline void CONFIG_RD15_AS_DIG_INPUT() {
#ifdef _CNPUD15
  _CNPUD15 = 0; //disable pullup
#endif
#ifdef _CNPDD15
  _CNPDD15 = 0; //disable pulldown
#endif
#ifdef _TRISD15
  _TRISD15 = 1;
#endif
#ifdef _ANSD15
  _ANSD15 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RD15
#ifdef _RE0
#ifdef _CNPUE0
#define ENABLE_RE0_PULLUP() _CNPUE0 = 1
#define DISABLE_RE0_PULLUP() _CNPUE0 = 0
#endif
#ifdef _CNPDE0
#define ENABLE_RE0_PULLDOWN() _CNPDE0 = 1
#define DISABLE_RE0_PULLDOWN() _CNPDE0 = 0
#endif
#ifdef _ANSE0
#define ENABLE_RE0_ANALOG() _ANSE0 = 1
#define DISABLE_RE0_ANALOG() _ANSE0 = 0
#endif
#ifdef _CNIEE0
#define ENABLE_RE0_CN_INTERRUPT() _CNIEE0 = 1
#define DISABLE_RE0_CN_INTERRUPT() _CNIEE0 = 0
#endif
static inline void CONFIG_RE0_AS_DIG_OUTPUT() {
#ifdef _CNPUE0
  _CNPUE0 = 0; //disable pullup
#endif
#ifdef _CNPDE0
  _CNPDE0 = 0; //disable pulldown
#endif
#ifdef _ODCE0
  _ODCE0 = 0;
#endif
#ifdef _TRISE0
  _TRISE0 = 0;
#endif
#ifdef _ANSE0
  _ANSE0 = 0;  //disable analog
#endif
}
#ifdef _ODCE0
#define ENABLE_RE0_OPENDRAIN() _ODCE0 = 1
#define DISABLE_RE0_OPENDRAIN() _ODCE0 = 0
#define CONFIG_RE0_AS_DIG_OD_OUTPUT()  {CONFIG_RE0_AS_DIG_OUTPUT();ENABLE_RE0_OPENDRAIN();}
#endif
static inline void CONFIG_RE0_AS_DIG_INPUT() {
#ifdef _CNPUE0
  _CNPUE0 = 0; //disable pullup
#endif
#ifdef _CNPDE0
  _CNPDE0 = 0; //disable pulldown
#endif
#ifdef _TRISE0
  _TRISE0 = 1;
#endif
#ifdef _ANSE0
  _ANSE0 = 0;  //disable analog
#endif
}
#define CONFIG_AN24_AS_DIGITAL() _ANSE0 = 0
#define CONFIG_AN24_AS_ANALOG() _ANSE0 = 1
#endif  //endif #ifdef _RE0
#ifdef _RE1
#ifdef _CNPUE1
#define ENABLE_RE1_PULLUP() _CNPUE1 = 1
#define DISABLE_RE1_PULLUP() _CNPUE1 = 0
#endif
#ifdef _CNPDE1
#define ENABLE_RE1_PULLDOWN() _CNPDE1 = 1
#define DISABLE_RE1_PULLDOWN() _CNPDE1 = 0
#endif
#ifdef _ANSE1
#define ENABLE_RE1_ANALOG() _ANSE1 = 1
#define DISABLE_RE1_ANALOG() _ANSE1 = 0
#endif
#ifdef _CNIEE1
#define ENABLE_RE1_CN_INTERRUPT() _CNIEE1 = 1
#define DISABLE_RE1_CN_INTERRUPT() _CNIEE1 = 0
#endif
static inline void CONFIG_RE1_AS_DIG_OUTPUT() {
#ifdef _CNPUE1
  _CNPUE1 = 0; //disable pullup
#endif
#ifdef _CNPDE1
  _CNPDE1 = 0; //disable pulldown
#endif
#ifdef _ODCE1
  _ODCE1 = 0;
#endif
#ifdef _TRISE1
  _TRISE1 = 0;
#endif
#ifdef _ANSE1
  _ANSE1 = 0;  //disable analog
#endif
}
#ifdef _ODCE1
#define ENABLE_RE1_OPENDRAIN() _ODCE1 = 1
#define DISABLE_RE1_OPENDRAIN() _ODCE1 = 0
#define CONFIG_RE1_AS_DIG_OD_OUTPUT()  {CONFIG_RE1_AS_DIG_OUTPUT();ENABLE_RE1_OPENDRAIN();}
#endif
static inline void CONFIG_RE1_AS_DIG_INPUT() {
#ifdef _CNPUE1
  _CNPUE1 = 0; //disable pullup
#endif
#ifdef _CNPDE1
  _CNPDE1 = 0; //disable pulldown
#endif
#ifdef _TRISE1
  _TRISE1 = 1;
#endif
#ifdef _ANSE1
  _ANSE1 = 0;  //disable analog
#endif
}
#define CONFIG_AN25_AS_DIGITAL() _ANSE1 = 0
#define CONFIG_AN25_AS_ANALOG() _ANSE1 = 1
#endif  //endif #ifdef _RE1
#ifdef _RE2
#ifdef _CNPUE2
#define ENABLE_RE2_PULLUP() _CNPUE2 = 1
#define DISABLE_RE2_PULLUP() _CNPUE2 = 0
#endif
#ifdef _CNPDE2
#define ENABLE_RE2_PULLDOWN() _CNPDE2 = 1
#define DISABLE_RE2_PULLDOWN() _CNPDE2 = 0
#endif
#ifdef _ANSE2
#define ENABLE_RE2_ANALOG() _ANSE2 = 1
#define DISABLE_RE2_ANALOG() _ANSE2 = 0
#endif
#ifdef _CNIEE2
#define ENABLE_RE2_CN_INTERRUPT() _CNIEE2 = 1
#define DISABLE_RE2_CN_INTERRUPT() _CNIEE2 = 0
#endif
static inline void CONFIG_RE2_AS_DIG_OUTPUT() {
#ifdef _CNPUE2
  _CNPUE2 = 0; //disable pullup
#endif
#ifdef _CNPDE2
  _CNPDE2 = 0; //disable pulldown
#endif
#ifdef _ODCE2
  _ODCE2 = 0;
#endif
#ifdef _TRISE2
  _TRISE2 = 0;
#endif
#ifdef _ANSE2
  _ANSE2 = 0;  //disable analog
#endif
}
#ifdef _ODCE2
#define ENABLE_RE2_OPENDRAIN() _ODCE2 = 1
#define DISABLE_RE2_OPENDRAIN() _ODCE2 = 0
#define CONFIG_RE2_AS_DIG_OD_OUTPUT()  {CONFIG_RE2_AS_DIG_OUTPUT();ENABLE_RE2_OPENDRAIN();}
#endif
static inline void CONFIG_RE2_AS_DIG_INPUT() {
#ifdef _CNPUE2
  _CNPUE2 = 0; //disable pullup
#endif
#ifdef _CNPDE2
  _CNPDE2 = 0; //disable pulldown
#endif
#ifdef _TRISE2
  _TRISE2 = 1;
#endif
#ifdef _ANSE2
  _ANSE2 = 0;  //disable analog
#endif
}
#define CONFIG_AN26_AS_DIGITAL() _ANSE2 = 0
#define CONFIG_AN26_AS_ANALOG() _ANSE2 = 1
#endif  //endif #ifdef _RE2
#ifdef _RE3
#ifdef _CNPUE3
#define ENABLE_RE3_PULLUP() _CNPUE3 = 1
#define DISABLE_RE3_PULLUP() _CNPUE3 = 0
#endif
#ifdef _CNPDE3
#define ENABLE_RE3_PULLDOWN() _CNPDE3 = 1
#define DISABLE_RE3_PULLDOWN() _CNPDE3 = 0
#endif
#ifdef _ANSE3
#define ENABLE_RE3_ANALOG() _ANSE3 = 1
#define DISABLE_RE3_ANALOG() _ANSE3 = 0
#endif
#ifdef _CNIEE3
#define ENABLE_RE3_CN_INTERRUPT() _CNIEE3 = 1
#define DISABLE_RE3_CN_INTERRUPT() _CNIEE3 = 0
#endif
static inline void CONFIG_RE3_AS_DIG_OUTPUT() {
#ifdef _CNPUE3
  _CNPUE3 = 0; //disable pullup
#endif
#ifdef _CNPDE3
  _CNPDE3 = 0; //disable pulldown
#endif
#ifdef _ODCE3
  _ODCE3 = 0;
#endif
#ifdef _TRISE3
  _TRISE3 = 0;
#endif
#ifdef _ANSE3
  _ANSE3 = 0;  //disable analog
#endif
}
#ifdef _ODCE3
#define ENABLE_RE3_OPENDRAIN() _ODCE3 = 1
#define DISABLE_RE3_OPENDRAIN() _ODCE3 = 0
#define CONFIG_RE3_AS_DIG_OD_OUTPUT()  {CONFIG_RE3_AS_DIG_OUTPUT();ENABLE_RE3_OPENDRAIN();}
#endif
static inline void CONFIG_RE3_AS_DIG_INPUT() {
#ifdef _CNPUE3
  _CNPUE3 = 0; //disable pullup
#endif
#ifdef _CNPDE3
  _CNPDE3 = 0; //disable pulldown
#endif
#ifdef _TRISE3
  _TRISE3 = 1;
#endif
#ifdef _ANSE3
  _ANSE3 = 0;  //disable analog
#endif
}
#define CONFIG_AN27_AS_DIGITAL() _ANSE3 = 0
#define CONFIG_AN27_AS_ANALOG() _ANSE3 = 1
#endif  //endif #ifdef _RE3
#ifdef _RE4
#ifdef _CNPUE4
#define ENABLE_RE4_PULLUP() _CNPUE4 = 1
#define DISABLE_RE4_PULLUP() _CNPUE4 = 0
#endif
#ifdef _CNPDE4
#define ENABLE_RE4_PULLDOWN() _CNPDE4 = 1
#define DISABLE_RE4_PULLDOWN() _CNPDE4 = 0
#endif
#ifdef _ANSE4
#define ENABLE_RE4_ANALOG() _ANSE4 = 1
#define DISABLE_RE4_ANALOG() _ANSE4 = 0
#endif
#ifdef _CNIEE4
#define ENABLE_RE4_CN_INTERRUPT() _CNIEE4 = 1
#define DISABLE_RE4_CN_INTERRUPT() _CNIEE4 = 0
#endif
static inline void CONFIG_RE4_AS_DIG_OUTPUT() {
#ifdef _CNPUE4
  _CNPUE4 = 0; //disable pullup
#endif
#ifdef _CNPDE4
  _CNPDE4 = 0; //disable pulldown
#endif
#ifdef _ODCE4
  _ODCE4 = 0;
#endif
#ifdef _TRISE4
  _TRISE4 = 0;
#endif
#ifdef _ANSE4
  _ANSE4 = 0;  //disable analog
#endif
}
#ifdef _ODCE4
#define ENABLE_RE4_OPENDRAIN() _ODCE4 = 1
#define DISABLE_RE4_OPENDRAIN() _ODCE4 = 0
#define CONFIG_RE4_AS_DIG_OD_OUTPUT()  {CONFIG_RE4_AS_DIG_OUTPUT();ENABLE_RE4_OPENDRAIN();}
#endif
static inline void CONFIG_RE4_AS_DIG_INPUT() {
#ifdef _CNPUE4
  _CNPUE4 = 0; //disable pullup
#endif
#ifdef _CNPDE4
  _CNPDE4 = 0; //disable pulldown
#endif
#ifdef _TRISE4
  _TRISE4 = 1;
#endif
#ifdef _ANSE4
  _ANSE4 = 0;  //disable analog
#endif
}
#define CONFIG_AN28_AS_DIGITAL() _ANSE4 = 0
#define CONFIG_AN28_AS_ANALOG() _ANSE4 = 1
#endif  //endif #ifdef _RE4
#ifdef _RE5
#ifdef _CNPUE5
#define ENABLE_RE5_PULLUP() _CNPUE5 = 1
#define DISABLE_RE5_PULLUP() _CNPUE5 = 0
#endif
#ifdef _CNPDE5
#define ENABLE_RE5_PULLDOWN() _CNPDE5 = 1
#define DISABLE_RE5_PULLDOWN() _CNPDE5 = 0
#endif
#ifdef _ANSE5
#define ENABLE_RE5_ANALOG() _ANSE5 = 1
#define DISABLE_RE5_ANALOG() _ANSE5 = 0
#endif
#ifdef _CNIEE5
#define ENABLE_RE5_CN_INTERRUPT() _CNIEE5 = 1
#define DISABLE_RE5_CN_INTERRUPT() _CNIEE5 = 0
#endif
static inline void CONFIG_RE5_AS_DIG_OUTPUT() {
#ifdef _CNPUE5
  _CNPUE5 = 0; //disable pullup
#endif
#ifdef _CNPDE5
  _CNPDE5 = 0; //disable pulldown
#endif
#ifdef _ODCE5
  _ODCE5 = 0;
#endif
#ifdef _TRISE5
  _TRISE5 = 0;
#endif
#ifdef _ANSE5
  _ANSE5 = 0;  //disable analog
#endif
}
#ifdef _ODCE5
#define ENABLE_RE5_OPENDRAIN() _ODCE5 = 1
#define DISABLE_RE5_OPENDRAIN() _ODCE5 = 0
#define CONFIG_RE5_AS_DIG_OD_OUTPUT()  {CONFIG_RE5_AS_DIG_OUTPUT();ENABLE_RE5_OPENDRAIN();}
#endif
static inline void CONFIG_RE5_AS_DIG_INPUT() {
#ifdef _CNPUE5
  _CNPUE5 = 0; //disable pullup
#endif
#ifdef _CNPDE5
  _CNPDE5 = 0; //disable pulldown
#endif
#ifdef _TRISE5
  _TRISE5 = 1;
#endif
#ifdef _ANSE5
  _ANSE5 = 0;  //disable analog
#endif
}
#define CONFIG_AN29_AS_DIGITAL() _ANSE5 = 0
#define CONFIG_AN29_AS_ANALOG() _ANSE5 = 1
#endif  //endif #ifdef _RE5
#ifdef _RE6
#ifdef _CNPUE6
#define ENABLE_RE6_PULLUP() _CNPUE6 = 1
#define DISABLE_RE6_PULLUP() _CNPUE6 = 0
#endif
#ifdef _CNPDE6
#define ENABLE_RE6_PULLDOWN() _CNPDE6 = 1
#define DISABLE_RE6_PULLDOWN() _CNPDE6 = 0
#endif
#ifdef _ANSE6
#define ENABLE_RE6_ANALOG() _ANSE6 = 1
#define DISABLE_RE6_ANALOG() _ANSE6 = 0
#endif
#ifdef _CNIEE6
#define ENABLE_RE6_CN_INTERRUPT() _CNIEE6 = 1
#define DISABLE_RE6_CN_INTERRUPT() _CNIEE6 = 0
#endif
static inline void CONFIG_RE6_AS_DIG_OUTPUT() {
#ifdef _CNPUE6
  _CNPUE6 = 0; //disable pullup
#endif
#ifdef _CNPDE6
  _CNPDE6 = 0; //disable pulldown
#endif
#ifdef _ODCE6
  _ODCE6 = 0;
#endif
#ifdef _TRISE6
  _TRISE6 = 0;
#endif
#ifdef _ANSE6
  _ANSE6 = 0;  //disable analog
#endif
}
#ifdef _ODCE6
#define ENABLE_RE6_OPENDRAIN() _ODCE6 = 1
#define DISABLE_RE6_OPENDRAIN() _ODCE6 = 0
#define CONFIG_RE6_AS_DIG_OD_OUTPUT()  {CONFIG_RE6_AS_DIG_OUTPUT();ENABLE_RE6_OPENDRAIN();}
#endif
static inline void CONFIG_RE6_AS_DIG_INPUT() {
#ifdef _CNPUE6
  _CNPUE6 = 0; //disable pullup
#endif
#ifdef _CNPDE6
  _CNPDE6 = 0; //disable pulldown
#endif
#ifdef _TRISE6
  _TRISE6 = 1;
#endif
#ifdef _ANSE6
  _ANSE6 = 0;  //disable analog
#endif
}
#define CONFIG_AN30_AS_DIGITAL() _ANSE6 = 0
#define CONFIG_AN30_AS_ANALOG() _ANSE6 = 1
#endif  //endif #ifdef _RE6
#ifdef _RE7
#ifdef _CNPUE7
#define ENABLE_RE7_PULLUP() _CNPUE7 = 1
#define DISABLE_RE7_PULLUP() _CNPUE7 = 0
#endif
#ifdef _CNPDE7
#define ENABLE_RE7_PULLDOWN() _CNPDE7 = 1
#define DISABLE_RE7_PULLDOWN() _CNPDE7 = 0
#endif
#ifdef _ANSE7
#define ENABLE_RE7_ANALOG() _ANSE7 = 1
#define DISABLE_RE7_ANALOG() _ANSE7 = 0
#endif
#ifdef _CNIEE7
#define ENABLE_RE7_CN_INTERRUPT() _CNIEE7 = 1
#define DISABLE_RE7_CN_INTERRUPT() _CNIEE7 = 0
#endif
static inline void CONFIG_RE7_AS_DIG_OUTPUT() {
#ifdef _CNPUE7
  _CNPUE7 = 0; //disable pullup
#endif
#ifdef _CNPDE7
  _CNPDE7 = 0; //disable pulldown
#endif
#ifdef _ODCE7
  _ODCE7 = 0;
#endif
#ifdef _TRISE7
  _TRISE7 = 0;
#endif
#ifdef _ANSE7
  _ANSE7 = 0;  //disable analog
#endif
}
#ifdef _ODCE7
#define ENABLE_RE7_OPENDRAIN() _ODCE7 = 1
#define DISABLE_RE7_OPENDRAIN() _ODCE7 = 0
#define CONFIG_RE7_AS_DIG_OD_OUTPUT()  {CONFIG_RE7_AS_DIG_OUTPUT();ENABLE_RE7_OPENDRAIN();}
#endif
static inline void CONFIG_RE7_AS_DIG_INPUT() {
#ifdef _CNPUE7
  _CNPUE7 = 0; //disable pullup
#endif
#ifdef _CNPDE7
  _CNPDE7 = 0; //disable pulldown
#endif
#ifdef _TRISE7
  _TRISE7 = 1;
#endif
#ifdef _ANSE7
  _ANSE7 = 0;  //disable analog
#endif
}
#define CONFIG_AN31_AS_DIGITAL() _ANSE7 = 0
#define CONFIG_AN31_AS_ANALOG() _ANSE7 = 1
#endif  //endif #ifdef _RE7
#ifdef _RE8
#ifdef _CNPUE8
#define ENABLE_RE8_PULLUP() _CNPUE8 = 1
#define DISABLE_RE8_PULLUP() _CNPUE8 = 0
#endif
#ifdef _CNPDE8
#define ENABLE_RE8_PULLDOWN() _CNPDE8 = 1
#define DISABLE_RE8_PULLDOWN() _CNPDE8 = 0
#endif
#ifdef _ANSE8
#define ENABLE_RE8_ANALOG() _ANSE8 = 1
#define DISABLE_RE8_ANALOG() _ANSE8 = 0
#endif
#ifdef _CNIEE8
#define ENABLE_RE8_CN_INTERRUPT() _CNIEE8 = 1
#define DISABLE_RE8_CN_INTERRUPT() _CNIEE8 = 0
#endif
static inline void CONFIG_RE8_AS_DIG_OUTPUT() {
#ifdef _CNPUE8
  _CNPUE8 = 0; //disable pullup
#endif
#ifdef _CNPDE8
  _CNPDE8 = 0; //disable pulldown
#endif
#ifdef _ODCE8
  _ODCE8 = 0;
#endif
#ifdef _TRISE8
  _TRISE8 = 0;
#endif
#ifdef _ANSE8
  _ANSE8 = 0;  //disable analog
#endif
}
#ifdef _ODCE8
#define ENABLE_RE8_OPENDRAIN() _ODCE8 = 1
#define DISABLE_RE8_OPENDRAIN() _ODCE8 = 0
#define CONFIG_RE8_AS_DIG_OD_OUTPUT()  {CONFIG_RE8_AS_DIG_OUTPUT();ENABLE_RE8_OPENDRAIN();}
#endif
static inline void CONFIG_RE8_AS_DIG_INPUT() {
#ifdef _CNPUE8
  _CNPUE8 = 0; //disable pullup
#endif
#ifdef _CNPDE8
  _CNPDE8 = 0; //disable pulldown
#endif
#ifdef _TRISE8
  _TRISE8 = 1;
#endif
#ifdef _ANSE8
  _ANSE8 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RE8
#ifdef _RE9
#ifdef _CNPUE9
#define ENABLE_RE9_PULLUP() _CNPUE9 = 1
#define DISABLE_RE9_PULLUP() _CNPUE9 = 0
#endif
#ifdef _CNPDE9
#define ENABLE_RE9_PULLDOWN() _CNPDE9 = 1
#define DISABLE_RE9_PULLDOWN() _CNPDE9 = 0
#endif
#ifdef _ANSE9
#define ENABLE_RE9_ANALOG() _ANSE9 = 1
#define DISABLE_RE9_ANALOG() _ANSE9 = 0
#endif
#ifdef _CNIEE9
#define ENABLE_RE9_CN_INTERRUPT() _CNIEE9 = 1
#define DISABLE_RE9_CN_INTERRUPT() _CNIEE9 = 0
#endif
static inline void CONFIG_RE9_AS_DIG_OUTPUT() {
#ifdef _CNPUE9
  _CNPUE9 = 0; //disable pullup
#endif
#ifdef _CNPDE9
  _CNPDE9 = 0; //disable pulldown
#endif
#ifdef _ODCE9
  _ODCE9 = 0;
#endif
#ifdef _TRISE9
  _TRISE9 = 0;
#endif
#ifdef _ANSE9
  _ANSE9 = 0;  //disable analog
#endif
}
#ifdef _ODCE9
#define ENABLE_RE9_OPENDRAIN() _ODCE9 = 1
#define DISABLE_RE9_OPENDRAIN() _ODCE9 = 0
#define CONFIG_RE9_AS_DIG_OD_OUTPUT()  {CONFIG_RE9_AS_DIG_OUTPUT();ENABLE_RE9_OPENDRAIN();}
#endif
static inline void CONFIG_RE9_AS_DIG_INPUT() {
#ifdef _CNPUE9
  _CNPUE9 = 0; //disable pullup
#endif
#ifdef _CNPDE9
  _CNPDE9 = 0; //disable pulldown
#endif
#ifdef _TRISE9
  _TRISE9 = 1;
#endif
#ifdef _ANSE9
  _ANSE9 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RE9
#ifdef _RE10
#ifdef _CNPUE10
#define ENABLE_RE10_PULLUP() _CNPUE10 = 1
#define DISABLE_RE10_PULLUP() _CNPUE10 = 0
#endif
#ifdef _CNPDE10
#define ENABLE_RE10_PULLDOWN() _CNPDE10 = 1
#define DISABLE_RE10_PULLDOWN() _CNPDE10 = 0
#endif
#ifdef _ANSE10
#define ENABLE_RE10_ANALOG() _ANSE10 = 1
#define DISABLE_RE10_ANALOG() _ANSE10 = 0
#endif
#ifdef _CNIEE10
#define ENABLE_RE10_CN_INTERRUPT() _CNIEE10 = 1
#define DISABLE_RE10_CN_INTERRUPT() _CNIEE10 = 0
#endif
static inline void CONFIG_RE10_AS_DIG_OUTPUT() {
#ifdef _CNPUE10
  _CNPUE10 = 0; //disable pullup
#endif
#ifdef _CNPDE10
  _CNPDE10 = 0; //disable pulldown
#endif
#ifdef _ODCE10
  _ODCE10 = 0;
#endif
#ifdef _TRISE10
  _TRISE10 = 0;
#endif
#ifdef _ANSE10
  _ANSE10 = 0;  //disable analog
#endif
}
#ifdef _ODCE10
#define ENABLE_RE10_OPENDRAIN() _ODCE10 = 1
#define DISABLE_RE10_OPENDRAIN() _ODCE10 = 0
#define CONFIG_RE10_AS_DIG_OD_OUTPUT()  {CONFIG_RE10_AS_DIG_OUTPUT();ENABLE_RE10_OPENDRAIN();}
#endif
static inline void CONFIG_RE10_AS_DIG_INPUT() {
#ifdef _CNPUE10
  _CNPUE10 = 0; //disable pullup
#endif
#ifdef _CNPDE10
  _CNPDE10 = 0; //disable pulldown
#endif
#ifdef _TRISE10
  _TRISE10 = 1;
#endif
#ifdef _ANSE10
  _ANSE10 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RE10
#ifdef _RE11
#ifdef _CNPUE11
#define ENABLE_RE11_PULLUP() _CNPUE11 = 1
#define DISABLE_RE11_PULLUP() _CNPUE11 = 0
#endif
#ifdef _CNPDE11
#define ENABLE_RE11_PULLDOWN() _CNPDE11 = 1
#define DISABLE_RE11_PULLDOWN() _CNPDE11 = 0
#endif
#ifdef _ANSE11
#define ENABLE_RE11_ANALOG() _ANSE11 = 1
#define DISABLE_RE11_ANALOG() _ANSE11 = 0
#endif
#ifdef _CNIEE11
#define ENABLE_RE11_CN_INTERRUPT() _CNIEE11 = 1
#define DISABLE_RE11_CN_INTERRUPT() _CNIEE11 = 0
#endif
static inline void CONFIG_RE11_AS_DIG_OUTPUT() {
#ifdef _CNPUE11
  _CNPUE11 = 0; //disable pullup
#endif
#ifdef _CNPDE11
  _CNPDE11 = 0; //disable pulldown
#endif
#ifdef _ODCE11
  _ODCE11 = 0;
#endif
#ifdef _TRISE11
  _TRISE11 = 0;
#endif
#ifdef _ANSE11
  _ANSE11 = 0;  //disable analog
#endif
}
#ifdef _ODCE11
#define ENABLE_RE11_OPENDRAIN() _ODCE11 = 1
#define DISABLE_RE11_OPENDRAIN() _ODCE11 = 0
#define CONFIG_RE11_AS_DIG_OD_OUTPUT()  {CONFIG_RE11_AS_DIG_OUTPUT();ENABLE_RE11_OPENDRAIN();}
#endif
static inline void CONFIG_RE11_AS_DIG_INPUT() {
#ifdef _CNPUE11
  _CNPUE11 = 0; //disable pullup
#endif
#ifdef _CNPDE11
  _CNPDE11 = 0; //disable pulldown
#endif
#ifdef _TRISE11
  _TRISE11 = 1;
#endif
#ifdef _ANSE11
  _ANSE11 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RE11
#ifdef _RE12
#ifdef _CNPUE12
#define ENABLE_RE12_PULLUP() _CNPUE12 = 1
#define DISABLE_RE12_PULLUP() _CNPUE12 = 0
#endif
#ifdef _CNPDE12
#define ENABLE_RE12_PULLDOWN() _CNPDE12 = 1
#define DISABLE_RE12_PULLDOWN() _CNPDE12 = 0
#endif
#ifdef _ANSE12
#define ENABLE_RE12_ANALOG() _ANSE12 = 1
#define DISABLE_RE12_ANALOG() _ANSE12 = 0
#endif
#ifdef _CNIEE12
#define ENABLE_RE12_CN_INTERRUPT() _CNIEE12 = 1
#define DISABLE_RE12_CN_INTERRUPT() _CNIEE12 = 0
#endif
static inline void CONFIG_RE12_AS_DIG_OUTPUT() {
#ifdef _CNPUE12
  _CNPUE12 = 0; //disable pullup
#endif
#ifdef _CNPDE12
  _CNPDE12 = 0; //disable pulldown
#endif
#ifdef _ODCE12
  _ODCE12 = 0;
#endif
#ifdef _TRISE12
  _TRISE12 = 0;
#endif
#ifdef _ANSE12
  _ANSE12 = 0;  //disable analog
#endif
}
#ifdef _ODCE12
#define ENABLE_RE12_OPENDRAIN() _ODCE12 = 1
#define DISABLE_RE12_OPENDRAIN() _ODCE12 = 0
#define CONFIG_RE12_AS_DIG_OD_OUTPUT()  {CONFIG_RE12_AS_DIG_OUTPUT();ENABLE_RE12_OPENDRAIN();}
#endif
static inline void CONFIG_RE12_AS_DIG_INPUT() {
#ifdef _CNPUE12
  _CNPUE12 = 0; //disable pullup
#endif
#ifdef _CNPDE12
  _CNPDE12 = 0; //disable pulldown
#endif
#ifdef _TRISE12
  _TRISE12 = 1;
#endif
#ifdef _ANSE12
  _ANSE12 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RE12
#ifdef _RE13
#ifdef _CNPUE13
#define ENABLE_RE13_PULLUP() _CNPUE13 = 1
#define DISABLE_RE13_PULLUP() _CNPUE13 = 0
#endif
#ifdef _CNPDE13
#define ENABLE_RE13_PULLDOWN() _CNPDE13 = 1
#define DISABLE_RE13_PULLDOWN() _CNPDE13 = 0
#endif
#ifdef _ANSE13
#define ENABLE_RE13_ANALOG() _ANSE13 = 1
#define DISABLE_RE13_ANALOG() _ANSE13 = 0
#endif
#ifdef _CNIEE13
#define ENABLE_RE13_CN_INTERRUPT() _CNIEE13 = 1
#define DISABLE_RE13_CN_INTERRUPT() _CNIEE13 = 0
#endif
static inline void CONFIG_RE13_AS_DIG_OUTPUT() {
#ifdef _CNPUE13
  _CNPUE13 = 0; //disable pullup
#endif
#ifdef _CNPDE13
  _CNPDE13 = 0; //disable pulldown
#endif
#ifdef _ODCE13
  _ODCE13 = 0;
#endif
#ifdef _TRISE13
  _TRISE13 = 0;
#endif
#ifdef _ANSE13
  _ANSE13 = 0;  //disable analog
#endif
}
#ifdef _ODCE13
#define ENABLE_RE13_OPENDRAIN() _ODCE13 = 1
#define DISABLE_RE13_OPENDRAIN() _ODCE13 = 0
#define CONFIG_RE13_AS_DIG_OD_OUTPUT()  {CONFIG_RE13_AS_DIG_OUTPUT();ENABLE_RE13_OPENDRAIN();}
#endif
static inline void CONFIG_RE13_AS_DIG_INPUT() {
#ifdef _CNPUE13
  _CNPUE13 = 0; //disable pullup
#endif
#ifdef _CNPDE13
  _CNPDE13 = 0; //disable pulldown
#endif
#ifdef _TRISE13
  _TRISE13 = 1;
#endif
#ifdef _ANSE13
  _ANSE13 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RE13
#ifdef _RE14
#ifdef _CNPUE14
#define ENABLE_RE14_PULLUP() _CNPUE14 = 1
#define DISABLE_RE14_PULLUP() _CNPUE14 = 0
#endif
#ifdef _CNPDE14
#define ENABLE_RE14_PULLDOWN() _CNPDE14 = 1
#define DISABLE_RE14_PULLDOWN() _CNPDE14 = 0
#endif
#ifdef _ANSE14
#define ENABLE_RE14_ANALOG() _ANSE14 = 1
#define DISABLE_RE14_ANALOG() _ANSE14 = 0
#endif
#ifdef _CNIEE14
#define ENABLE_RE14_CN_INTERRUPT() _CNIEE14 = 1
#define DISABLE_RE14_CN_INTERRUPT() _CNIEE14 = 0
#endif
static inline void CONFIG_RE14_AS_DIG_OUTPUT() {
#ifdef _CNPUE14
  _CNPUE14 = 0; //disable pullup
#endif
#ifdef _CNPDE14
  _CNPDE14 = 0; //disable pulldown
#endif
#ifdef _ODCE14
  _ODCE14 = 0;
#endif
#ifdef _TRISE14
  _TRISE14 = 0;
#endif
#ifdef _ANSE14
  _ANSE14 = 0;  //disable analog
#endif
}
#ifdef _ODCE14
#define ENABLE_RE14_OPENDRAIN() _ODCE14 = 1
#define DISABLE_RE14_OPENDRAIN() _ODCE14 = 0
#define CONFIG_RE14_AS_DIG_OD_OUTPUT()  {CONFIG_RE14_AS_DIG_OUTPUT();ENABLE_RE14_OPENDRAIN();}
#endif
static inline void CONFIG_RE14_AS_DIG_INPUT() {
#ifdef _CNPUE14
  _CNPUE14 = 0; //disable pullup
#endif
#ifdef _CNPDE14
  _CNPDE14 = 0; //disable pulldown
#endif
#ifdef _TRISE14
  _TRISE14 = 1;
#endif
#ifdef _ANSE14
  _ANSE14 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RE14
#ifdef _RE15
#ifdef _CNPUE15
#define ENABLE_RE15_PULLUP() _CNPUE15 = 1
#define DISABLE_RE15_PULLUP() _CNPUE15 = 0
#endif
#ifdef _CNPDE15
#define ENABLE_RE15_PULLDOWN() _CNPDE15 = 1
#define DISABLE_RE15_PULLDOWN() _CNPDE15 = 0
#endif
#ifdef _ANSE15
#define ENABLE_RE15_ANALOG() _ANSE15 = 1
#define DISABLE_RE15_ANALOG() _ANSE15 = 0
#endif
#ifdef _CNIEE15
#define ENABLE_RE15_CN_INTERRUPT() _CNIEE15 = 1
#define DISABLE_RE15_CN_INTERRUPT() _CNIEE15 = 0
#endif
static inline void CONFIG_RE15_AS_DIG_OUTPUT() {
#ifdef _CNPUE15
  _CNPUE15 = 0; //disable pullup
#endif
#ifdef _CNPDE15
  _CNPDE15 = 0; //disable pulldown
#endif
#ifdef _ODCE15
  _ODCE15 = 0;
#endif
#ifdef _TRISE15
  _TRISE15 = 0;
#endif
#ifdef _ANSE15
  _ANSE15 = 0;  //disable analog
#endif
}
#ifdef _ODCE15
#define ENABLE_RE15_OPENDRAIN() _ODCE15 = 1
#define DISABLE_RE15_OPENDRAIN() _ODCE15 = 0
#define CONFIG_RE15_AS_DIG_OD_OUTPUT()  {CONFIG_RE15_AS_DIG_OUTPUT();ENABLE_RE15_OPENDRAIN();}
#endif
static inline void CONFIG_RE15_AS_DIG_INPUT() {
#ifdef _CNPUE15
  _CNPUE15 = 0; //disable pullup
#endif
#ifdef _CNPDE15
  _CNPDE15 = 0; //disable pulldown
#endif
#ifdef _TRISE15
  _TRISE15 = 1;
#endif
#ifdef _ANSE15
  _ANSE15 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RE15
#ifdef _RF0
#ifdef _CNPUF0
#define ENABLE_RF0_PULLUP() _CNPUF0 = 1
#define DISABLE_RF0_PULLUP() _CNPUF0 = 0
#endif
#ifdef _CNPDF0
#define ENABLE_RF0_PULLDOWN() _CNPDF0 = 1
#define DISABLE_RF0_PULLDOWN() _CNPDF0 = 0
#endif
#ifdef _ANSF0
#define ENABLE_RF0_ANALOG() _ANSF0 = 1
#define DISABLE_RF0_ANALOG() _ANSF0 = 0
#endif
#ifdef _CNIEF0
#define ENABLE_RF0_CN_INTERRUPT() _CNIEF0 = 1
#define DISABLE_RF0_CN_INTERRUPT() _CNIEF0 = 0
#endif
static inline void CONFIG_RF0_AS_DIG_OUTPUT() {
#ifdef _CNPUF0
  _CNPUF0 = 0; //disable pullup
#endif
#ifdef _CNPDF0
  _CNPDF0 = 0; //disable pulldown
#endif
#ifdef _ODCF0
  _ODCF0 = 0;
#endif
#ifdef _TRISF0
  _TRISF0 = 0;
#endif
#ifdef _ANSF0
  _ANSF0 = 0;  //disable analog
#endif
}
#ifdef _ODCF0
#define ENABLE_RF0_OPENDRAIN() _ODCF0 = 1
#define DISABLE_RF0_OPENDRAIN() _ODCF0 = 0
#define CONFIG_RF0_AS_DIG_OD_OUTPUT()  {CONFIG_RF0_AS_DIG_OUTPUT();ENABLE_RF0_OPENDRAIN();}
#endif
static inline void CONFIG_RF0_AS_DIG_INPUT() {
#ifdef _CNPUF0
  _CNPUF0 = 0; //disable pullup
#endif
#ifdef _CNPDF0
  _CNPDF0 = 0; //disable pulldown
#endif
#ifdef _TRISF0
  _TRISF0 = 1;
#endif
#ifdef _ANSF0
  _ANSF0 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF0
#ifdef _RF1
#ifdef _CNPUF1
#define ENABLE_RF1_PULLUP() _CNPUF1 = 1
#define DISABLE_RF1_PULLUP() _CNPUF1 = 0
#endif
#ifdef _CNPDF1
#define ENABLE_RF1_PULLDOWN() _CNPDF1 = 1
#define DISABLE_RF1_PULLDOWN() _CNPDF1 = 0
#endif
#ifdef _ANSF1
#define ENABLE_RF1_ANALOG() _ANSF1 = 1
#define DISABLE_RF1_ANALOG() _ANSF1 = 0
#endif
#ifdef _CNIEF1
#define ENABLE_RF1_CN_INTERRUPT() _CNIEF1 = 1
#define DISABLE_RF1_CN_INTERRUPT() _CNIEF1 = 0
#endif
static inline void CONFIG_RF1_AS_DIG_OUTPUT() {
#ifdef _CNPUF1
  _CNPUF1 = 0; //disable pullup
#endif
#ifdef _CNPDF1
  _CNPDF1 = 0; //disable pulldown
#endif
#ifdef _ODCF1
  _ODCF1 = 0;
#endif
#ifdef _TRISF1
  _TRISF1 = 0;
#endif
#ifdef _ANSF1
  _ANSF1 = 0;  //disable analog
#endif
}
#ifdef _ODCF1
#define ENABLE_RF1_OPENDRAIN() _ODCF1 = 1
#define DISABLE_RF1_OPENDRAIN() _ODCF1 = 0
#define CONFIG_RF1_AS_DIG_OD_OUTPUT()  {CONFIG_RF1_AS_DIG_OUTPUT();ENABLE_RF1_OPENDRAIN();}
#endif
static inline void CONFIG_RF1_AS_DIG_INPUT() {
#ifdef _CNPUF1
  _CNPUF1 = 0; //disable pullup
#endif
#ifdef _CNPDF1
  _CNPDF1 = 0; //disable pulldown
#endif
#ifdef _TRISF1
  _TRISF1 = 1;
#endif
#ifdef _ANSF1
  _ANSF1 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF1
#ifdef _RF2
#ifdef _CNPUF2
#define ENABLE_RF2_PULLUP() _CNPUF2 = 1
#define DISABLE_RF2_PULLUP() _CNPUF2 = 0
#endif
#ifdef _CNPDF2
#define ENABLE_RF2_PULLDOWN() _CNPDF2 = 1
#define DISABLE_RF2_PULLDOWN() _CNPDF2 = 0
#endif
#ifdef _ANSF2
#define ENABLE_RF2_ANALOG() _ANSF2 = 1
#define DISABLE_RF2_ANALOG() _ANSF2 = 0
#endif
#ifdef _CNIEF2
#define ENABLE_RF2_CN_INTERRUPT() _CNIEF2 = 1
#define DISABLE_RF2_CN_INTERRUPT() _CNIEF2 = 0
#endif
static inline void CONFIG_RF2_AS_DIG_OUTPUT() {
#ifdef _CNPUF2
  _CNPUF2 = 0; //disable pullup
#endif
#ifdef _CNPDF2
  _CNPDF2 = 0; //disable pulldown
#endif
#ifdef _ODCF2
  _ODCF2 = 0;
#endif
#ifdef _TRISF2
  _TRISF2 = 0;
#endif
#ifdef _ANSF2
  _ANSF2 = 0;  //disable analog
#endif
}
#ifdef _ODCF2
#define ENABLE_RF2_OPENDRAIN() _ODCF2 = 1
#define DISABLE_RF2_OPENDRAIN() _ODCF2 = 0
#define CONFIG_RF2_AS_DIG_OD_OUTPUT()  {CONFIG_RF2_AS_DIG_OUTPUT();ENABLE_RF2_OPENDRAIN();}
#endif
static inline void CONFIG_RF2_AS_DIG_INPUT() {
#ifdef _CNPUF2
  _CNPUF2 = 0; //disable pullup
#endif
#ifdef _CNPDF2
  _CNPDF2 = 0; //disable pulldown
#endif
#ifdef _TRISF2
  _TRISF2 = 1;
#endif
#ifdef _ANSF2
  _ANSF2 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF2
#ifdef _RF3
#ifdef _CNPUF3
#define ENABLE_RF3_PULLUP() _CNPUF3 = 1
#define DISABLE_RF3_PULLUP() _CNPUF3 = 0
#endif
#ifdef _CNPDF3
#define ENABLE_RF3_PULLDOWN() _CNPDF3 = 1
#define DISABLE_RF3_PULLDOWN() _CNPDF3 = 0
#endif
#ifdef _ANSF3
#define ENABLE_RF3_ANALOG() _ANSF3 = 1
#define DISABLE_RF3_ANALOG() _ANSF3 = 0
#endif
#ifdef _CNIEF3
#define ENABLE_RF3_CN_INTERRUPT() _CNIEF3 = 1
#define DISABLE_RF3_CN_INTERRUPT() _CNIEF3 = 0
#endif
static inline void CONFIG_RF3_AS_DIG_OUTPUT() {
#ifdef _CNPUF3
  _CNPUF3 = 0; //disable pullup
#endif
#ifdef _CNPDF3
  _CNPDF3 = 0; //disable pulldown
#endif
#ifdef _ODCF3
  _ODCF3 = 0;
#endif
#ifdef _TRISF3
  _TRISF3 = 0;
#endif
#ifdef _ANSF3
  _ANSF3 = 0;  //disable analog
#endif
}
#ifdef _ODCF3
#define ENABLE_RF3_OPENDRAIN() _ODCF3 = 1
#define DISABLE_RF3_OPENDRAIN() _ODCF3 = 0
#define CONFIG_RF3_AS_DIG_OD_OUTPUT()  {CONFIG_RF3_AS_DIG_OUTPUT();ENABLE_RF3_OPENDRAIN();}
#endif
static inline void CONFIG_RF3_AS_DIG_INPUT() {
#ifdef _CNPUF3
  _CNPUF3 = 0; //disable pullup
#endif
#ifdef _CNPDF3
  _CNPDF3 = 0; //disable pulldown
#endif
#ifdef _TRISF3
  _TRISF3 = 1;
#endif
#ifdef _ANSF3
  _ANSF3 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF3
#ifdef _RF4
#ifdef _CNPUF4
#define ENABLE_RF4_PULLUP() _CNPUF4 = 1
#define DISABLE_RF4_PULLUP() _CNPUF4 = 0
#endif
#ifdef _CNPDF4
#define ENABLE_RF4_PULLDOWN() _CNPDF4 = 1
#define DISABLE_RF4_PULLDOWN() _CNPDF4 = 0
#endif
#ifdef _ANSF4
#define ENABLE_RF4_ANALOG() _ANSF4 = 1
#define DISABLE_RF4_ANALOG() _ANSF4 = 0
#endif
#ifdef _CNIEF4
#define ENABLE_RF4_CN_INTERRUPT() _CNIEF4 = 1
#define DISABLE_RF4_CN_INTERRUPT() _CNIEF4 = 0
#endif
static inline void CONFIG_RF4_AS_DIG_OUTPUT() {
#ifdef _CNPUF4
  _CNPUF4 = 0; //disable pullup
#endif
#ifdef _CNPDF4
  _CNPDF4 = 0; //disable pulldown
#endif
#ifdef _ODCF4
  _ODCF4 = 0;
#endif
#ifdef _TRISF4
  _TRISF4 = 0;
#endif
#ifdef _ANSF4
  _ANSF4 = 0;  //disable analog
#endif
}
#ifdef _ODCF4
#define ENABLE_RF4_OPENDRAIN() _ODCF4 = 1
#define DISABLE_RF4_OPENDRAIN() _ODCF4 = 0
#define CONFIG_RF4_AS_DIG_OD_OUTPUT()  {CONFIG_RF4_AS_DIG_OUTPUT();ENABLE_RF4_OPENDRAIN();}
#endif
static inline void CONFIG_RF4_AS_DIG_INPUT() {
#ifdef _CNPUF4
  _CNPUF4 = 0; //disable pullup
#endif
#ifdef _CNPDF4
  _CNPDF4 = 0; //disable pulldown
#endif
#ifdef _TRISF4
  _TRISF4 = 1;
#endif
#ifdef _ANSF4
  _ANSF4 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF4
#ifdef _RF5
#ifdef _CNPUF5
#define ENABLE_RF5_PULLUP() _CNPUF5 = 1
#define DISABLE_RF5_PULLUP() _CNPUF5 = 0
#endif
#ifdef _CNPDF5
#define ENABLE_RF5_PULLDOWN() _CNPDF5 = 1
#define DISABLE_RF5_PULLDOWN() _CNPDF5 = 0
#endif
#ifdef _ANSF5
#define ENABLE_RF5_ANALOG() _ANSF5 = 1
#define DISABLE_RF5_ANALOG() _ANSF5 = 0
#endif
#ifdef _CNIEF5
#define ENABLE_RF5_CN_INTERRUPT() _CNIEF5 = 1
#define DISABLE_RF5_CN_INTERRUPT() _CNIEF5 = 0
#endif
static inline void CONFIG_RF5_AS_DIG_OUTPUT() {
#ifdef _CNPUF5
  _CNPUF5 = 0; //disable pullup
#endif
#ifdef _CNPDF5
  _CNPDF5 = 0; //disable pulldown
#endif
#ifdef _ODCF5
  _ODCF5 = 0;
#endif
#ifdef _TRISF5
  _TRISF5 = 0;
#endif
#ifdef _ANSF5
  _ANSF5 = 0;  //disable analog
#endif
}
#ifdef _ODCF5
#define ENABLE_RF5_OPENDRAIN() _ODCF5 = 1
#define DISABLE_RF5_OPENDRAIN() _ODCF5 = 0
#define CONFIG_RF5_AS_DIG_OD_OUTPUT()  {CONFIG_RF5_AS_DIG_OUTPUT();ENABLE_RF5_OPENDRAIN();}
#endif
static inline void CONFIG_RF5_AS_DIG_INPUT() {
#ifdef _CNPUF5
  _CNPUF5 = 0; //disable pullup
#endif
#ifdef _CNPDF5
  _CNPDF5 = 0; //disable pulldown
#endif
#ifdef _TRISF5
  _TRISF5 = 1;
#endif
#ifdef _ANSF5
  _ANSF5 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF5
#ifdef _RF6
#ifdef _CNPUF6
#define ENABLE_RF6_PULLUP() _CNPUF6 = 1
#define DISABLE_RF6_PULLUP() _CNPUF6 = 0
#endif
#ifdef _CNPDF6
#define ENABLE_RF6_PULLDOWN() _CNPDF6 = 1
#define DISABLE_RF6_PULLDOWN() _CNPDF6 = 0
#endif
#ifdef _ANSF6
#define ENABLE_RF6_ANALOG() _ANSF6 = 1
#define DISABLE_RF6_ANALOG() _ANSF6 = 0
#endif
#ifdef _CNIEF6
#define ENABLE_RF6_CN_INTERRUPT() _CNIEF6 = 1
#define DISABLE_RF6_CN_INTERRUPT() _CNIEF6 = 0
#endif
static inline void CONFIG_RF6_AS_DIG_OUTPUT() {
#ifdef _CNPUF6
  _CNPUF6 = 0; //disable pullup
#endif
#ifdef _CNPDF6
  _CNPDF6 = 0; //disable pulldown
#endif
#ifdef _ODCF6
  _ODCF6 = 0;
#endif
#ifdef _TRISF6
  _TRISF6 = 0;
#endif
#ifdef _ANSF6
  _ANSF6 = 0;  //disable analog
#endif
}
#ifdef _ODCF6
#define ENABLE_RF6_OPENDRAIN() _ODCF6 = 1
#define DISABLE_RF6_OPENDRAIN() _ODCF6 = 0
#define CONFIG_RF6_AS_DIG_OD_OUTPUT()  {CONFIG_RF6_AS_DIG_OUTPUT();ENABLE_RF6_OPENDRAIN();}
#endif
static inline void CONFIG_RF6_AS_DIG_INPUT() {
#ifdef _CNPUF6
  _CNPUF6 = 0; //disable pullup
#endif
#ifdef _CNPDF6
  _CNPDF6 = 0; //disable pulldown
#endif
#ifdef _TRISF6
  _TRISF6 = 1;
#endif
#ifdef _ANSF6
  _ANSF6 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF6
#ifdef _RF7
#ifdef _CNPUF7
#define ENABLE_RF7_PULLUP() _CNPUF7 = 1
#define DISABLE_RF7_PULLUP() _CNPUF7 = 0
#endif
#ifdef _CNPDF7
#define ENABLE_RF7_PULLDOWN() _CNPDF7 = 1
#define DISABLE_RF7_PULLDOWN() _CNPDF7 = 0
#endif
#ifdef _ANSF7
#define ENABLE_RF7_ANALOG() _ANSF7 = 1
#define DISABLE_RF7_ANALOG() _ANSF7 = 0
#endif
#ifdef _CNIEF7
#define ENABLE_RF7_CN_INTERRUPT() _CNIEF7 = 1
#define DISABLE_RF7_CN_INTERRUPT() _CNIEF7 = 0
#endif
static inline void CONFIG_RF7_AS_DIG_OUTPUT() {
#ifdef _CNPUF7
  _CNPUF7 = 0; //disable pullup
#endif
#ifdef _CNPDF7
  _CNPDF7 = 0; //disable pulldown
#endif
#ifdef _ODCF7
  _ODCF7 = 0;
#endif
#ifdef _TRISF7
  _TRISF7 = 0;
#endif
#ifdef _ANSF7
  _ANSF7 = 0;  //disable analog
#endif
}
#ifdef _ODCF7
#define ENABLE_RF7_OPENDRAIN() _ODCF7 = 1
#define DISABLE_RF7_OPENDRAIN() _ODCF7 = 0
#define CONFIG_RF7_AS_DIG_OD_OUTPUT()  {CONFIG_RF7_AS_DIG_OUTPUT();ENABLE_RF7_OPENDRAIN();}
#endif
static inline void CONFIG_RF7_AS_DIG_INPUT() {
#ifdef _CNPUF7
  _CNPUF7 = 0; //disable pullup
#endif
#ifdef _CNPDF7
  _CNPDF7 = 0; //disable pulldown
#endif
#ifdef _TRISF7
  _TRISF7 = 1;
#endif
#ifdef _ANSF7
  _ANSF7 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF7
#ifdef _RF8
#ifdef _CNPUF8
#define ENABLE_RF8_PULLUP() _CNPUF8 = 1
#define DISABLE_RF8_PULLUP() _CNPUF8 = 0
#endif
#ifdef _CNPDF8
#define ENABLE_RF8_PULLDOWN() _CNPDF8 = 1
#define DISABLE_RF8_PULLDOWN() _CNPDF8 = 0
#endif
#ifdef _ANSF8
#define ENABLE_RF8_ANALOG() _ANSF8 = 1
#define DISABLE_RF8_ANALOG() _ANSF8 = 0
#endif
#ifdef _CNIEF8
#define ENABLE_RF8_CN_INTERRUPT() _CNIEF8 = 1
#define DISABLE_RF8_CN_INTERRUPT() _CNIEF8 = 0
#endif
static inline void CONFIG_RF8_AS_DIG_OUTPUT() {
#ifdef _CNPUF8
  _CNPUF8 = 0; //disable pullup
#endif
#ifdef _CNPDF8
  _CNPDF8 = 0; //disable pulldown
#endif
#ifdef _ODCF8
  _ODCF8 = 0;
#endif
#ifdef _TRISF8
  _TRISF8 = 0;
#endif
#ifdef _ANSF8
  _ANSF8 = 0;  //disable analog
#endif
}
#ifdef _ODCF8
#define ENABLE_RF8_OPENDRAIN() _ODCF8 = 1
#define DISABLE_RF8_OPENDRAIN() _ODCF8 = 0
#define CONFIG_RF8_AS_DIG_OD_OUTPUT()  {CONFIG_RF8_AS_DIG_OUTPUT();ENABLE_RF8_OPENDRAIN();}
#endif
static inline void CONFIG_RF8_AS_DIG_INPUT() {
#ifdef _CNPUF8
  _CNPUF8 = 0; //disable pullup
#endif
#ifdef _CNPDF8
  _CNPDF8 = 0; //disable pulldown
#endif
#ifdef _TRISF8
  _TRISF8 = 1;
#endif
#ifdef _ANSF8
  _ANSF8 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF8
#ifdef _RF9
#ifdef _CNPUF9
#define ENABLE_RF9_PULLUP() _CNPUF9 = 1
#define DISABLE_RF9_PULLUP() _CNPUF9 = 0
#endif
#ifdef _CNPDF9
#define ENABLE_RF9_PULLDOWN() _CNPDF9 = 1
#define DISABLE_RF9_PULLDOWN() _CNPDF9 = 0
#endif
#ifdef _ANSF9
#define ENABLE_RF9_ANALOG() _ANSF9 = 1
#define DISABLE_RF9_ANALOG() _ANSF9 = 0
#endif
#ifdef _CNIEF9
#define ENABLE_RF9_CN_INTERRUPT() _CNIEF9 = 1
#define DISABLE_RF9_CN_INTERRUPT() _CNIEF9 = 0
#endif
static inline void CONFIG_RF9_AS_DIG_OUTPUT() {
#ifdef _CNPUF9
  _CNPUF9 = 0; //disable pullup
#endif
#ifdef _CNPDF9
  _CNPDF9 = 0; //disable pulldown
#endif
#ifdef _ODCF9
  _ODCF9 = 0;
#endif
#ifdef _TRISF9
  _TRISF9 = 0;
#endif
#ifdef _ANSF9
  _ANSF9 = 0;  //disable analog
#endif
}
#ifdef _ODCF9
#define ENABLE_RF9_OPENDRAIN() _ODCF9 = 1
#define DISABLE_RF9_OPENDRAIN() _ODCF9 = 0
#define CONFIG_RF9_AS_DIG_OD_OUTPUT()  {CONFIG_RF9_AS_DIG_OUTPUT();ENABLE_RF9_OPENDRAIN();}
#endif
static inline void CONFIG_RF9_AS_DIG_INPUT() {
#ifdef _CNPUF9
  _CNPUF9 = 0; //disable pullup
#endif
#ifdef _CNPDF9
  _CNPDF9 = 0; //disable pulldown
#endif
#ifdef _TRISF9
  _TRISF9 = 1;
#endif
#ifdef _ANSF9
  _ANSF9 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF9
#ifdef _RF10
#ifdef _CNPUF10
#define ENABLE_RF10_PULLUP() _CNPUF10 = 1
#define DISABLE_RF10_PULLUP() _CNPUF10 = 0
#endif
#ifdef _CNPDF10
#define ENABLE_RF10_PULLDOWN() _CNPDF10 = 1
#define DISABLE_RF10_PULLDOWN() _CNPDF10 = 0
#endif
#ifdef _ANSF10
#define ENABLE_RF10_ANALOG() _ANSF10 = 1
#define DISABLE_RF10_ANALOG() _ANSF10 = 0
#endif
#ifdef _CNIEF10
#define ENABLE_RF10_CN_INTERRUPT() _CNIEF10 = 1
#define DISABLE_RF10_CN_INTERRUPT() _CNIEF10 = 0
#endif
static inline void CONFIG_RF10_AS_DIG_OUTPUT() {
#ifdef _CNPUF10
  _CNPUF10 = 0; //disable pullup
#endif
#ifdef _CNPDF10
  _CNPDF10 = 0; //disable pulldown
#endif
#ifdef _ODCF10
  _ODCF10 = 0;
#endif
#ifdef _TRISF10
  _TRISF10 = 0;
#endif
#ifdef _ANSF10
  _ANSF10 = 0;  //disable analog
#endif
}
#ifdef _ODCF10
#define ENABLE_RF10_OPENDRAIN() _ODCF10 = 1
#define DISABLE_RF10_OPENDRAIN() _ODCF10 = 0
#define CONFIG_RF10_AS_DIG_OD_OUTPUT()  {CONFIG_RF10_AS_DIG_OUTPUT();ENABLE_RF10_OPENDRAIN();}
#endif
static inline void CONFIG_RF10_AS_DIG_INPUT() {
#ifdef _CNPUF10
  _CNPUF10 = 0; //disable pullup
#endif
#ifdef _CNPDF10
  _CNPDF10 = 0; //disable pulldown
#endif
#ifdef _TRISF10
  _TRISF10 = 1;
#endif
#ifdef _ANSF10
  _ANSF10 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF10
#ifdef _RF11
#ifdef _CNPUF11
#define ENABLE_RF11_PULLUP() _CNPUF11 = 1
#define DISABLE_RF11_PULLUP() _CNPUF11 = 0
#endif
#ifdef _CNPDF11
#define ENABLE_RF11_PULLDOWN() _CNPDF11 = 1
#define DISABLE_RF11_PULLDOWN() _CNPDF11 = 0
#endif
#ifdef _ANSF11
#define ENABLE_RF11_ANALOG() _ANSF11 = 1
#define DISABLE_RF11_ANALOG() _ANSF11 = 0
#endif
#ifdef _CNIEF11
#define ENABLE_RF11_CN_INTERRUPT() _CNIEF11 = 1
#define DISABLE_RF11_CN_INTERRUPT() _CNIEF11 = 0
#endif
static inline void CONFIG_RF11_AS_DIG_OUTPUT() {
#ifdef _CNPUF11
  _CNPUF11 = 0; //disable pullup
#endif
#ifdef _CNPDF11
  _CNPDF11 = 0; //disable pulldown
#endif
#ifdef _ODCF11
  _ODCF11 = 0;
#endif
#ifdef _TRISF11
  _TRISF11 = 0;
#endif
#ifdef _ANSF11
  _ANSF11 = 0;  //disable analog
#endif
}
#ifdef _ODCF11
#define ENABLE_RF11_OPENDRAIN() _ODCF11 = 1
#define DISABLE_RF11_OPENDRAIN() _ODCF11 = 0
#define CONFIG_RF11_AS_DIG_OD_OUTPUT()  {CONFIG_RF11_AS_DIG_OUTPUT();ENABLE_RF11_OPENDRAIN();}
#endif
static inline void CONFIG_RF11_AS_DIG_INPUT() {
#ifdef _CNPUF11
  _CNPUF11 = 0; //disable pullup
#endif
#ifdef _CNPDF11
  _CNPDF11 = 0; //disable pulldown
#endif
#ifdef _TRISF11
  _TRISF11 = 1;
#endif
#ifdef _ANSF11
  _ANSF11 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF11
#ifdef _RF12
#ifdef _CNPUF12
#define ENABLE_RF12_PULLUP() _CNPUF12 = 1
#define DISABLE_RF12_PULLUP() _CNPUF12 = 0
#endif
#ifdef _CNPDF12
#define ENABLE_RF12_PULLDOWN() _CNPDF12 = 1
#define DISABLE_RF12_PULLDOWN() _CNPDF12 = 0
#endif
#ifdef _ANSF12
#define ENABLE_RF12_ANALOG() _ANSF12 = 1
#define DISABLE_RF12_ANALOG() _ANSF12 = 0
#endif
#ifdef _CNIEF12
#define ENABLE_RF12_CN_INTERRUPT() _CNIEF12 = 1
#define DISABLE_RF12_CN_INTERRUPT() _CNIEF12 = 0
#endif
static inline void CONFIG_RF12_AS_DIG_OUTPUT() {
#ifdef _CNPUF12
  _CNPUF12 = 0; //disable pullup
#endif
#ifdef _CNPDF12
  _CNPDF12 = 0; //disable pulldown
#endif
#ifdef _ODCF12
  _ODCF12 = 0;
#endif
#ifdef _TRISF12
  _TRISF12 = 0;
#endif
#ifdef _ANSF12
  _ANSF12 = 0;  //disable analog
#endif
}
#ifdef _ODCF12
#define ENABLE_RF12_OPENDRAIN() _ODCF12 = 1
#define DISABLE_RF12_OPENDRAIN() _ODCF12 = 0
#define CONFIG_RF12_AS_DIG_OD_OUTPUT()  {CONFIG_RF12_AS_DIG_OUTPUT();ENABLE_RF12_OPENDRAIN();}
#endif
static inline void CONFIG_RF12_AS_DIG_INPUT() {
#ifdef _CNPUF12
  _CNPUF12 = 0; //disable pullup
#endif
#ifdef _CNPDF12
  _CNPDF12 = 0; //disable pulldown
#endif
#ifdef _TRISF12
  _TRISF12 = 1;
#endif
#ifdef _ANSF12
  _ANSF12 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF12
#ifdef _RF13
#ifdef _CNPUF13
#define ENABLE_RF13_PULLUP() _CNPUF13 = 1
#define DISABLE_RF13_PULLUP() _CNPUF13 = 0
#endif
#ifdef _CNPDF13
#define ENABLE_RF13_PULLDOWN() _CNPDF13 = 1
#define DISABLE_RF13_PULLDOWN() _CNPDF13 = 0
#endif
#ifdef _ANSF13
#define ENABLE_RF13_ANALOG() _ANSF13 = 1
#define DISABLE_RF13_ANALOG() _ANSF13 = 0
#endif
#ifdef _CNIEF13
#define ENABLE_RF13_CN_INTERRUPT() _CNIEF13 = 1
#define DISABLE_RF13_CN_INTERRUPT() _CNIEF13 = 0
#endif
static inline void CONFIG_RF13_AS_DIG_OUTPUT() {
#ifdef _CNPUF13
  _CNPUF13 = 0; //disable pullup
#endif
#ifdef _CNPDF13
  _CNPDF13 = 0; //disable pulldown
#endif
#ifdef _ODCF13
  _ODCF13 = 0;
#endif
#ifdef _TRISF13
  _TRISF13 = 0;
#endif
#ifdef _ANSF13
  _ANSF13 = 0;  //disable analog
#endif
}
#ifdef _ODCF13
#define ENABLE_RF13_OPENDRAIN() _ODCF13 = 1
#define DISABLE_RF13_OPENDRAIN() _ODCF13 = 0
#define CONFIG_RF13_AS_DIG_OD_OUTPUT()  {CONFIG_RF13_AS_DIG_OUTPUT();ENABLE_RF13_OPENDRAIN();}
#endif
static inline void CONFIG_RF13_AS_DIG_INPUT() {
#ifdef _CNPUF13
  _CNPUF13 = 0; //disable pullup
#endif
#ifdef _CNPDF13
  _CNPDF13 = 0; //disable pulldown
#endif
#ifdef _TRISF13
  _TRISF13 = 1;
#endif
#ifdef _ANSF13
  _ANSF13 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF13
#ifdef _RF14
#ifdef _CNPUF14
#define ENABLE_RF14_PULLUP() _CNPUF14 = 1
#define DISABLE_RF14_PULLUP() _CNPUF14 = 0
#endif
#ifdef _CNPDF14
#define ENABLE_RF14_PULLDOWN() _CNPDF14 = 1
#define DISABLE_RF14_PULLDOWN() _CNPDF14 = 0
#endif
#ifdef _ANSF14
#define ENABLE_RF14_ANALOG() _ANSF14 = 1
#define DISABLE_RF14_ANALOG() _ANSF14 = 0
#endif
#ifdef _CNIEF14
#define ENABLE_RF14_CN_INTERRUPT() _CNIEF14 = 1
#define DISABLE_RF14_CN_INTERRUPT() _CNIEF14 = 0
#endif
static inline void CONFIG_RF14_AS_DIG_OUTPUT() {
#ifdef _CNPUF14
  _CNPUF14 = 0; //disable pullup
#endif
#ifdef _CNPDF14
  _CNPDF14 = 0; //disable pulldown
#endif
#ifdef _ODCF14
  _ODCF14 = 0;
#endif
#ifdef _TRISF14
  _TRISF14 = 0;
#endif
#ifdef _ANSF14
  _ANSF14 = 0;  //disable analog
#endif
}
#ifdef _ODCF14
#define ENABLE_RF14_OPENDRAIN() _ODCF14 = 1
#define DISABLE_RF14_OPENDRAIN() _ODCF14 = 0
#define CONFIG_RF14_AS_DIG_OD_OUTPUT()  {CONFIG_RF14_AS_DIG_OUTPUT();ENABLE_RF14_OPENDRAIN();}
#endif
static inline void CONFIG_RF14_AS_DIG_INPUT() {
#ifdef _CNPUF14
  _CNPUF14 = 0; //disable pullup
#endif
#ifdef _CNPDF14
  _CNPDF14 = 0; //disable pulldown
#endif
#ifdef _TRISF14
  _TRISF14 = 1;
#endif
#ifdef _ANSF14
  _ANSF14 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF14
#ifdef _RF15
#ifdef _CNPUF15
#define ENABLE_RF15_PULLUP() _CNPUF15 = 1
#define DISABLE_RF15_PULLUP() _CNPUF15 = 0
#endif
#ifdef _CNPDF15
#define ENABLE_RF15_PULLDOWN() _CNPDF15 = 1
#define DISABLE_RF15_PULLDOWN() _CNPDF15 = 0
#endif
#ifdef _ANSF15
#define ENABLE_RF15_ANALOG() _ANSF15 = 1
#define DISABLE_RF15_ANALOG() _ANSF15 = 0
#endif
#ifdef _CNIEF15
#define ENABLE_RF15_CN_INTERRUPT() _CNIEF15 = 1
#define DISABLE_RF15_CN_INTERRUPT() _CNIEF15 = 0
#endif
static inline void CONFIG_RF15_AS_DIG_OUTPUT() {
#ifdef _CNPUF15
  _CNPUF15 = 0; //disable pullup
#endif
#ifdef _CNPDF15
  _CNPDF15 = 0; //disable pulldown
#endif
#ifdef _ODCF15
  _ODCF15 = 0;
#endif
#ifdef _TRISF15
  _TRISF15 = 0;
#endif
#ifdef _ANSF15
  _ANSF15 = 0;  //disable analog
#endif
}
#ifdef _ODCF15
#define ENABLE_RF15_OPENDRAIN() _ODCF15 = 1
#define DISABLE_RF15_OPENDRAIN() _ODCF15 = 0
#define CONFIG_RF15_AS_DIG_OD_OUTPUT()  {CONFIG_RF15_AS_DIG_OUTPUT();ENABLE_RF15_OPENDRAIN();}
#endif
static inline void CONFIG_RF15_AS_DIG_INPUT() {
#ifdef _CNPUF15
  _CNPUF15 = 0; //disable pullup
#endif
#ifdef _CNPDF15
  _CNPDF15 = 0; //disable pulldown
#endif
#ifdef _TRISF15
  _TRISF15 = 1;
#endif
#ifdef _ANSF15
  _ANSF15 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RF15
#ifdef _RG0
#ifdef _CNPUG0
#define ENABLE_RG0_PULLUP() _CNPUG0 = 1
#define DISABLE_RG0_PULLUP() _CNPUG0 = 0
#endif
#ifdef _CNPDG0
#define ENABLE_RG0_PULLDOWN() _CNPDG0 = 1
#define DISABLE_RG0_PULLDOWN() _CNPDG0 = 0
#endif
#ifdef _ANSG0
#define ENABLE_RG0_ANALOG() _ANSG0 = 1
#define DISABLE_RG0_ANALOG() _ANSG0 = 0
#endif
#ifdef _CNIEG0
#define ENABLE_RG0_CN_INTERRUPT() _CNIEG0 = 1
#define DISABLE_RG0_CN_INTERRUPT() _CNIEG0 = 0
#endif
static inline void CONFIG_RG0_AS_DIG_OUTPUT() {
#ifdef _CNPUG0
  _CNPUG0 = 0; //disable pullup
#endif
#ifdef _CNPDG0
  _CNPDG0 = 0; //disable pulldown
#endif
#ifdef _ODCG0
  _ODCG0 = 0;
#endif
#ifdef _TRISG0
  _TRISG0 = 0;
#endif
#ifdef _ANSG0
  _ANSG0 = 0;  //disable analog
#endif
}
#ifdef _ODCG0
#define ENABLE_RG0_OPENDRAIN() _ODCG0 = 1
#define DISABLE_RG0_OPENDRAIN() _ODCG0 = 0
#define CONFIG_RG0_AS_DIG_OD_OUTPUT()  {CONFIG_RG0_AS_DIG_OUTPUT();ENABLE_RG0_OPENDRAIN();}
#endif
static inline void CONFIG_RG0_AS_DIG_INPUT() {
#ifdef _CNPUG0
  _CNPUG0 = 0; //disable pullup
#endif
#ifdef _CNPDG0
  _CNPDG0 = 0; //disable pulldown
#endif
#ifdef _TRISG0
  _TRISG0 = 1;
#endif
#ifdef _ANSG0
  _ANSG0 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG0
#ifdef _RG1
#ifdef _CNPUG1
#define ENABLE_RG1_PULLUP() _CNPUG1 = 1
#define DISABLE_RG1_PULLUP() _CNPUG1 = 0
#endif
#ifdef _CNPDG1
#define ENABLE_RG1_PULLDOWN() _CNPDG1 = 1
#define DISABLE_RG1_PULLDOWN() _CNPDG1 = 0
#endif
#ifdef _ANSG1
#define ENABLE_RG1_ANALOG() _ANSG1 = 1
#define DISABLE_RG1_ANALOG() _ANSG1 = 0
#endif
#ifdef _CNIEG1
#define ENABLE_RG1_CN_INTERRUPT() _CNIEG1 = 1
#define DISABLE_RG1_CN_INTERRUPT() _CNIEG1 = 0
#endif
static inline void CONFIG_RG1_AS_DIG_OUTPUT() {
#ifdef _CNPUG1
  _CNPUG1 = 0; //disable pullup
#endif
#ifdef _CNPDG1
  _CNPDG1 = 0; //disable pulldown
#endif
#ifdef _ODCG1
  _ODCG1 = 0;
#endif
#ifdef _TRISG1
  _TRISG1 = 0;
#endif
#ifdef _ANSG1
  _ANSG1 = 0;  //disable analog
#endif
}
#ifdef _ODCG1
#define ENABLE_RG1_OPENDRAIN() _ODCG1 = 1
#define DISABLE_RG1_OPENDRAIN() _ODCG1 = 0
#define CONFIG_RG1_AS_DIG_OD_OUTPUT()  {CONFIG_RG1_AS_DIG_OUTPUT();ENABLE_RG1_OPENDRAIN();}
#endif
static inline void CONFIG_RG1_AS_DIG_INPUT() {
#ifdef _CNPUG1
  _CNPUG1 = 0; //disable pullup
#endif
#ifdef _CNPDG1
  _CNPDG1 = 0; //disable pulldown
#endif
#ifdef _TRISG1
  _TRISG1 = 1;
#endif
#ifdef _ANSG1
  _ANSG1 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG1
#ifdef _RG2
#ifdef _CNPUG2
#define ENABLE_RG2_PULLUP() _CNPUG2 = 1
#define DISABLE_RG2_PULLUP() _CNPUG2 = 0
#endif
#ifdef _CNPDG2
#define ENABLE_RG2_PULLDOWN() _CNPDG2 = 1
#define DISABLE_RG2_PULLDOWN() _CNPDG2 = 0
#endif
#ifdef _ANSG2
#define ENABLE_RG2_ANALOG() _ANSG2 = 1
#define DISABLE_RG2_ANALOG() _ANSG2 = 0
#endif
#ifdef _CNIEG2
#define ENABLE_RG2_CN_INTERRUPT() _CNIEG2 = 1
#define DISABLE_RG2_CN_INTERRUPT() _CNIEG2 = 0
#endif
static inline void CONFIG_RG2_AS_DIG_OUTPUT() {
#ifdef _CNPUG2
  _CNPUG2 = 0; //disable pullup
#endif
#ifdef _CNPDG2
  _CNPDG2 = 0; //disable pulldown
#endif
#ifdef _ODCG2
  _ODCG2 = 0;
#endif
#ifdef _TRISG2
  _TRISG2 = 0;
#endif
#ifdef _ANSG2
  _ANSG2 = 0;  //disable analog
#endif
}
#ifdef _ODCG2
#define ENABLE_RG2_OPENDRAIN() _ODCG2 = 1
#define DISABLE_RG2_OPENDRAIN() _ODCG2 = 0
#define CONFIG_RG2_AS_DIG_OD_OUTPUT()  {CONFIG_RG2_AS_DIG_OUTPUT();ENABLE_RG2_OPENDRAIN();}
#endif
static inline void CONFIG_RG2_AS_DIG_INPUT() {
#ifdef _CNPUG2
  _CNPUG2 = 0; //disable pullup
#endif
#ifdef _CNPDG2
  _CNPDG2 = 0; //disable pulldown
#endif
#ifdef _TRISG2
  _TRISG2 = 1;
#endif
#ifdef _ANSG2
  _ANSG2 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG2
#ifdef _RG3
#ifdef _CNPUG3
#define ENABLE_RG3_PULLUP() _CNPUG3 = 1
#define DISABLE_RG3_PULLUP() _CNPUG3 = 0
#endif
#ifdef _CNPDG3
#define ENABLE_RG3_PULLDOWN() _CNPDG3 = 1
#define DISABLE_RG3_PULLDOWN() _CNPDG3 = 0
#endif
#ifdef _ANSG3
#define ENABLE_RG3_ANALOG() _ANSG3 = 1
#define DISABLE_RG3_ANALOG() _ANSG3 = 0
#endif
#ifdef _CNIEG3
#define ENABLE_RG3_CN_INTERRUPT() _CNIEG3 = 1
#define DISABLE_RG3_CN_INTERRUPT() _CNIEG3 = 0
#endif
static inline void CONFIG_RG3_AS_DIG_OUTPUT() {
#ifdef _CNPUG3
  _CNPUG3 = 0; //disable pullup
#endif
#ifdef _CNPDG3
  _CNPDG3 = 0; //disable pulldown
#endif
#ifdef _ODCG3
  _ODCG3 = 0;
#endif
#ifdef _TRISG3
  _TRISG3 = 0;
#endif
#ifdef _ANSG3
  _ANSG3 = 0;  //disable analog
#endif
}
#ifdef _ODCG3
#define ENABLE_RG3_OPENDRAIN() _ODCG3 = 1
#define DISABLE_RG3_OPENDRAIN() _ODCG3 = 0
#define CONFIG_RG3_AS_DIG_OD_OUTPUT()  {CONFIG_RG3_AS_DIG_OUTPUT();ENABLE_RG3_OPENDRAIN();}
#endif
static inline void CONFIG_RG3_AS_DIG_INPUT() {
#ifdef _CNPUG3
  _CNPUG3 = 0; //disable pullup
#endif
#ifdef _CNPDG3
  _CNPDG3 = 0; //disable pulldown
#endif
#ifdef _TRISG3
  _TRISG3 = 1;
#endif
#ifdef _ANSG3
  _ANSG3 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG3
#ifdef _RG4
#ifdef _CNPUG4
#define ENABLE_RG4_PULLUP() _CNPUG4 = 1
#define DISABLE_RG4_PULLUP() _CNPUG4 = 0
#endif
#ifdef _CNPDG4
#define ENABLE_RG4_PULLDOWN() _CNPDG4 = 1
#define DISABLE_RG4_PULLDOWN() _CNPDG4 = 0
#endif
#ifdef _ANSG4
#define ENABLE_RG4_ANALOG() _ANSG4 = 1
#define DISABLE_RG4_ANALOG() _ANSG4 = 0
#endif
#ifdef _CNIEG4
#define ENABLE_RG4_CN_INTERRUPT() _CNIEG4 = 1
#define DISABLE_RG4_CN_INTERRUPT() _CNIEG4 = 0
#endif
static inline void CONFIG_RG4_AS_DIG_OUTPUT() {
#ifdef _CNPUG4
  _CNPUG4 = 0; //disable pullup
#endif
#ifdef _CNPDG4
  _CNPDG4 = 0; //disable pulldown
#endif
#ifdef _ODCG4
  _ODCG4 = 0;
#endif
#ifdef _TRISG4
  _TRISG4 = 0;
#endif
#ifdef _ANSG4
  _ANSG4 = 0;  //disable analog
#endif
}
#ifdef _ODCG4
#define ENABLE_RG4_OPENDRAIN() _ODCG4 = 1
#define DISABLE_RG4_OPENDRAIN() _ODCG4 = 0
#define CONFIG_RG4_AS_DIG_OD_OUTPUT()  {CONFIG_RG4_AS_DIG_OUTPUT();ENABLE_RG4_OPENDRAIN();}
#endif
static inline void CONFIG_RG4_AS_DIG_INPUT() {
#ifdef _CNPUG4
  _CNPUG4 = 0; //disable pullup
#endif
#ifdef _CNPDG4
  _CNPDG4 = 0; //disable pulldown
#endif
#ifdef _TRISG4
  _TRISG4 = 1;
#endif
#ifdef _ANSG4
  _ANSG4 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG4
#ifdef _RG5
#ifdef _CNPUG5
#define ENABLE_RG5_PULLUP() _CNPUG5 = 1
#define DISABLE_RG5_PULLUP() _CNPUG5 = 0
#endif
#ifdef _CNPDG5
#define ENABLE_RG5_PULLDOWN() _CNPDG5 = 1
#define DISABLE_RG5_PULLDOWN() _CNPDG5 = 0
#endif
#ifdef _ANSG5
#define ENABLE_RG5_ANALOG() _ANSG5 = 1
#define DISABLE_RG5_ANALOG() _ANSG5 = 0
#endif
#ifdef _CNIEG5
#define ENABLE_RG5_CN_INTERRUPT() _CNIEG5 = 1
#define DISABLE_RG5_CN_INTERRUPT() _CNIEG5 = 0
#endif
static inline void CONFIG_RG5_AS_DIG_OUTPUT() {
#ifdef _CNPUG5
  _CNPUG5 = 0; //disable pullup
#endif
#ifdef _CNPDG5
  _CNPDG5 = 0; //disable pulldown
#endif
#ifdef _ODCG5
  _ODCG5 = 0;
#endif
#ifdef _TRISG5
  _TRISG5 = 0;
#endif
#ifdef _ANSG5
  _ANSG5 = 0;  //disable analog
#endif
}
#ifdef _ODCG5
#define ENABLE_RG5_OPENDRAIN() _ODCG5 = 1
#define DISABLE_RG5_OPENDRAIN() _ODCG5 = 0
#define CONFIG_RG5_AS_DIG_OD_OUTPUT()  {CONFIG_RG5_AS_DIG_OUTPUT();ENABLE_RG5_OPENDRAIN();}
#endif
static inline void CONFIG_RG5_AS_DIG_INPUT() {
#ifdef _CNPUG5
  _CNPUG5 = 0; //disable pullup
#endif
#ifdef _CNPDG5
  _CNPDG5 = 0; //disable pulldown
#endif
#ifdef _TRISG5
  _TRISG5 = 1;
#endif
#ifdef _ANSG5
  _ANSG5 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG5
#ifdef _RG6
#ifdef _CNPUG6
#define ENABLE_RG6_PULLUP() _CNPUG6 = 1
#define DISABLE_RG6_PULLUP() _CNPUG6 = 0
#endif
#ifdef _CNPDG6
#define ENABLE_RG6_PULLDOWN() _CNPDG6 = 1
#define DISABLE_RG6_PULLDOWN() _CNPDG6 = 0
#endif
#ifdef _ANSG6
#define ENABLE_RG6_ANALOG() _ANSG6 = 1
#define DISABLE_RG6_ANALOG() _ANSG6 = 0
#endif
#ifdef _CNIEG6
#define ENABLE_RG6_CN_INTERRUPT() _CNIEG6 = 1
#define DISABLE_RG6_CN_INTERRUPT() _CNIEG6 = 0
#endif
static inline void CONFIG_RG6_AS_DIG_OUTPUT() {
#ifdef _CNPUG6
  _CNPUG6 = 0; //disable pullup
#endif
#ifdef _CNPDG6
  _CNPDG6 = 0; //disable pulldown
#endif
#ifdef _ODCG6
  _ODCG6 = 0;
#endif
#ifdef _TRISG6
  _TRISG6 = 0;
#endif
#ifdef _ANSG6
  _ANSG6 = 0;  //disable analog
#endif
}
#ifdef _ODCG6
#define ENABLE_RG6_OPENDRAIN() _ODCG6 = 1
#define DISABLE_RG6_OPENDRAIN() _ODCG6 = 0
#define CONFIG_RG6_AS_DIG_OD_OUTPUT()  {CONFIG_RG6_AS_DIG_OUTPUT();ENABLE_RG6_OPENDRAIN();}
#endif
static inline void CONFIG_RG6_AS_DIG_INPUT() {
#ifdef _CNPUG6
  _CNPUG6 = 0; //disable pullup
#endif
#ifdef _CNPDG6
  _CNPDG6 = 0; //disable pulldown
#endif
#ifdef _TRISG6
  _TRISG6 = 1;
#endif
#ifdef _ANSG6
  _ANSG6 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG6
#ifdef _RG7
#ifdef _CNPUG7
#define ENABLE_RG7_PULLUP() _CNPUG7 = 1
#define DISABLE_RG7_PULLUP() _CNPUG7 = 0
#endif
#ifdef _CNPDG7
#define ENABLE_RG7_PULLDOWN() _CNPDG7 = 1
#define DISABLE_RG7_PULLDOWN() _CNPDG7 = 0
#endif
#ifdef _ANSG7
#define ENABLE_RG7_ANALOG() _ANSG7 = 1
#define DISABLE_RG7_ANALOG() _ANSG7 = 0
#endif
#ifdef _CNIEG7
#define ENABLE_RG7_CN_INTERRUPT() _CNIEG7 = 1
#define DISABLE_RG7_CN_INTERRUPT() _CNIEG7 = 0
#endif
static inline void CONFIG_RG7_AS_DIG_OUTPUT() {
#ifdef _CNPUG7
  _CNPUG7 = 0; //disable pullup
#endif
#ifdef _CNPDG7
  _CNPDG7 = 0; //disable pulldown
#endif
#ifdef _ODCG7
  _ODCG7 = 0;
#endif
#ifdef _TRISG7
  _TRISG7 = 0;
#endif
#ifdef _ANSG7
  _ANSG7 = 0;  //disable analog
#endif
}
#ifdef _ODCG7
#define ENABLE_RG7_OPENDRAIN() _ODCG7 = 1
#define DISABLE_RG7_OPENDRAIN() _ODCG7 = 0
#define CONFIG_RG7_AS_DIG_OD_OUTPUT()  {CONFIG_RG7_AS_DIG_OUTPUT();ENABLE_RG7_OPENDRAIN();}
#endif
static inline void CONFIG_RG7_AS_DIG_INPUT() {
#ifdef _CNPUG7
  _CNPUG7 = 0; //disable pullup
#endif
#ifdef _CNPDG7
  _CNPDG7 = 0; //disable pulldown
#endif
#ifdef _TRISG7
  _TRISG7 = 1;
#endif
#ifdef _ANSG7
  _ANSG7 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG7
#ifdef _RG8
#ifdef _CNPUG8
#define ENABLE_RG8_PULLUP() _CNPUG8 = 1
#define DISABLE_RG8_PULLUP() _CNPUG8 = 0
#endif
#ifdef _CNPDG8
#define ENABLE_RG8_PULLDOWN() _CNPDG8 = 1
#define DISABLE_RG8_PULLDOWN() _CNPDG8 = 0
#endif
#ifdef _ANSG8
#define ENABLE_RG8_ANALOG() _ANSG8 = 1
#define DISABLE_RG8_ANALOG() _ANSG8 = 0
#endif
#ifdef _CNIEG8
#define ENABLE_RG8_CN_INTERRUPT() _CNIEG8 = 1
#define DISABLE_RG8_CN_INTERRUPT() _CNIEG8 = 0
#endif
static inline void CONFIG_RG8_AS_DIG_OUTPUT() {
#ifdef _CNPUG8
  _CNPUG8 = 0; //disable pullup
#endif
#ifdef _CNPDG8
  _CNPDG8 = 0; //disable pulldown
#endif
#ifdef _ODCG8
  _ODCG8 = 0;
#endif
#ifdef _TRISG8
  _TRISG8 = 0;
#endif
#ifdef _ANSG8
  _ANSG8 = 0;  //disable analog
#endif
}
#ifdef _ODCG8
#define ENABLE_RG8_OPENDRAIN() _ODCG8 = 1
#define DISABLE_RG8_OPENDRAIN() _ODCG8 = 0
#define CONFIG_RG8_AS_DIG_OD_OUTPUT()  {CONFIG_RG8_AS_DIG_OUTPUT();ENABLE_RG8_OPENDRAIN();}
#endif
static inline void CONFIG_RG8_AS_DIG_INPUT() {
#ifdef _CNPUG8
  _CNPUG8 = 0; //disable pullup
#endif
#ifdef _CNPDG8
  _CNPDG8 = 0; //disable pulldown
#endif
#ifdef _TRISG8
  _TRISG8 = 1;
#endif
#ifdef _ANSG8
  _ANSG8 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG8
#ifdef _RG9
#ifdef _CNPUG9
#define ENABLE_RG9_PULLUP() _CNPUG9 = 1
#define DISABLE_RG9_PULLUP() _CNPUG9 = 0
#endif
#ifdef _CNPDG9
#define ENABLE_RG9_PULLDOWN() _CNPDG9 = 1
#define DISABLE_RG9_PULLDOWN() _CNPDG9 = 0
#endif
#ifdef _ANSG9
#define ENABLE_RG9_ANALOG() _ANSG9 = 1
#define DISABLE_RG9_ANALOG() _ANSG9 = 0
#endif
#ifdef _CNIEG9
#define ENABLE_RG9_CN_INTERRUPT() _CNIEG9 = 1
#define DISABLE_RG9_CN_INTERRUPT() _CNIEG9 = 0
#endif
static inline void CONFIG_RG9_AS_DIG_OUTPUT() {
#ifdef _CNPUG9
  _CNPUG9 = 0; //disable pullup
#endif
#ifdef _CNPDG9
  _CNPDG9 = 0; //disable pulldown
#endif
#ifdef _ODCG9
  _ODCG9 = 0;
#endif
#ifdef _TRISG9
  _TRISG9 = 0;
#endif
#ifdef _ANSG9
  _ANSG9 = 0;  //disable analog
#endif
}
#ifdef _ODCG9
#define ENABLE_RG9_OPENDRAIN() _ODCG9 = 1
#define DISABLE_RG9_OPENDRAIN() _ODCG9 = 0
#define CONFIG_RG9_AS_DIG_OD_OUTPUT()  {CONFIG_RG9_AS_DIG_OUTPUT();ENABLE_RG9_OPENDRAIN();}
#endif
static inline void CONFIG_RG9_AS_DIG_INPUT() {
#ifdef _CNPUG9
  _CNPUG9 = 0; //disable pullup
#endif
#ifdef _CNPDG9
  _CNPDG9 = 0; //disable pulldown
#endif
#ifdef _TRISG9
  _TRISG9 = 1;
#endif
#ifdef _ANSG9
  _ANSG9 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG9
#ifdef _RG10
#ifdef _CNPUG10
#define ENABLE_RG10_PULLUP() _CNPUG10 = 1
#define DISABLE_RG10_PULLUP() _CNPUG10 = 0
#endif
#ifdef _CNPDG10
#define ENABLE_RG10_PULLDOWN() _CNPDG10 = 1
#define DISABLE_RG10_PULLDOWN() _CNPDG10 = 0
#endif
#ifdef _ANSG10
#define ENABLE_RG10_ANALOG() _ANSG10 = 1
#define DISABLE_RG10_ANALOG() _ANSG10 = 0
#endif
#ifdef _CNIEG10
#define ENABLE_RG10_CN_INTERRUPT() _CNIEG10 = 1
#define DISABLE_RG10_CN_INTERRUPT() _CNIEG10 = 0
#endif
static inline void CONFIG_RG10_AS_DIG_OUTPUT() {
#ifdef _CNPUG10
  _CNPUG10 = 0; //disable pullup
#endif
#ifdef _CNPDG10
  _CNPDG10 = 0; //disable pulldown
#endif
#ifdef _ODCG10
  _ODCG10 = 0;
#endif
#ifdef _TRISG10
  _TRISG10 = 0;
#endif
#ifdef _ANSG10
  _ANSG10 = 0;  //disable analog
#endif
}
#ifdef _ODCG10
#define ENABLE_RG10_OPENDRAIN() _ODCG10 = 1
#define DISABLE_RG10_OPENDRAIN() _ODCG10 = 0
#define CONFIG_RG10_AS_DIG_OD_OUTPUT()  {CONFIG_RG10_AS_DIG_OUTPUT();ENABLE_RG10_OPENDRAIN();}
#endif
static inline void CONFIG_RG10_AS_DIG_INPUT() {
#ifdef _CNPUG10
  _CNPUG10 = 0; //disable pullup
#endif
#ifdef _CNPDG10
  _CNPDG10 = 0; //disable pulldown
#endif
#ifdef _TRISG10
  _TRISG10 = 1;
#endif
#ifdef _ANSG10
  _ANSG10 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG10
#ifdef _RG11
#ifdef _CNPUG11
#define ENABLE_RG11_PULLUP() _CNPUG11 = 1
#define DISABLE_RG11_PULLUP() _CNPUG11 = 0
#endif
#ifdef _CNPDG11
#define ENABLE_RG11_PULLDOWN() _CNPDG11 = 1
#define DISABLE_RG11_PULLDOWN() _CNPDG11 = 0
#endif
#ifdef _ANSG11
#define ENABLE_RG11_ANALOG() _ANSG11 = 1
#define DISABLE_RG11_ANALOG() _ANSG11 = 0
#endif
#ifdef _CNIEG11
#define ENABLE_RG11_CN_INTERRUPT() _CNIEG11 = 1
#define DISABLE_RG11_CN_INTERRUPT() _CNIEG11 = 0
#endif
static inline void CONFIG_RG11_AS_DIG_OUTPUT() {
#ifdef _CNPUG11
  _CNPUG11 = 0; //disable pullup
#endif
#ifdef _CNPDG11
  _CNPDG11 = 0; //disable pulldown
#endif
#ifdef _ODCG11
  _ODCG11 = 0;
#endif
#ifdef _TRISG11
  _TRISG11 = 0;
#endif
#ifdef _ANSG11
  _ANSG11 = 0;  //disable analog
#endif
}
#ifdef _ODCG11
#define ENABLE_RG11_OPENDRAIN() _ODCG11 = 1
#define DISABLE_RG11_OPENDRAIN() _ODCG11 = 0
#define CONFIG_RG11_AS_DIG_OD_OUTPUT()  {CONFIG_RG11_AS_DIG_OUTPUT();ENABLE_RG11_OPENDRAIN();}
#endif
static inline void CONFIG_RG11_AS_DIG_INPUT() {
#ifdef _CNPUG11
  _CNPUG11 = 0; //disable pullup
#endif
#ifdef _CNPDG11
  _CNPDG11 = 0; //disable pulldown
#endif
#ifdef _TRISG11
  _TRISG11 = 1;
#endif
#ifdef _ANSG11
  _ANSG11 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG11
#ifdef _RG12
#ifdef _CNPUG12
#define ENABLE_RG12_PULLUP() _CNPUG12 = 1
#define DISABLE_RG12_PULLUP() _CNPUG12 = 0
#endif
#ifdef _CNPDG12
#define ENABLE_RG12_PULLDOWN() _CNPDG12 = 1
#define DISABLE_RG12_PULLDOWN() _CNPDG12 = 0
#endif
#ifdef _ANSG12
#define ENABLE_RG12_ANALOG() _ANSG12 = 1
#define DISABLE_RG12_ANALOG() _ANSG12 = 0
#endif
#ifdef _CNIEG12
#define ENABLE_RG12_CN_INTERRUPT() _CNIEG12 = 1
#define DISABLE_RG12_CN_INTERRUPT() _CNIEG12 = 0
#endif
static inline void CONFIG_RG12_AS_DIG_OUTPUT() {
#ifdef _CNPUG12
  _CNPUG12 = 0; //disable pullup
#endif
#ifdef _CNPDG12
  _CNPDG12 = 0; //disable pulldown
#endif
#ifdef _ODCG12
  _ODCG12 = 0;
#endif
#ifdef _TRISG12
  _TRISG12 = 0;
#endif
#ifdef _ANSG12
  _ANSG12 = 0;  //disable analog
#endif
}
#ifdef _ODCG12
#define ENABLE_RG12_OPENDRAIN() _ODCG12 = 1
#define DISABLE_RG12_OPENDRAIN() _ODCG12 = 0
#define CONFIG_RG12_AS_DIG_OD_OUTPUT()  {CONFIG_RG12_AS_DIG_OUTPUT();ENABLE_RG12_OPENDRAIN();}
#endif
static inline void CONFIG_RG12_AS_DIG_INPUT() {
#ifdef _CNPUG12
  _CNPUG12 = 0; //disable pullup
#endif
#ifdef _CNPDG12
  _CNPDG12 = 0; //disable pulldown
#endif
#ifdef _TRISG12
  _TRISG12 = 1;
#endif
#ifdef _ANSG12
  _ANSG12 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG12
#ifdef _RG13
#ifdef _CNPUG13
#define ENABLE_RG13_PULLUP() _CNPUG13 = 1
#define DISABLE_RG13_PULLUP() _CNPUG13 = 0
#endif
#ifdef _CNPDG13
#define ENABLE_RG13_PULLDOWN() _CNPDG13 = 1
#define DISABLE_RG13_PULLDOWN() _CNPDG13 = 0
#endif
#ifdef _ANSG13
#define ENABLE_RG13_ANALOG() _ANSG13 = 1
#define DISABLE_RG13_ANALOG() _ANSG13 = 0
#endif
#ifdef _CNIEG13
#define ENABLE_RG13_CN_INTERRUPT() _CNIEG13 = 1
#define DISABLE_RG13_CN_INTERRUPT() _CNIEG13 = 0
#endif
static inline void CONFIG_RG13_AS_DIG_OUTPUT() {
#ifdef _CNPUG13
  _CNPUG13 = 0; //disable pullup
#endif
#ifdef _CNPDG13
  _CNPDG13 = 0; //disable pulldown
#endif
#ifdef _ODCG13
  _ODCG13 = 0;
#endif
#ifdef _TRISG13
  _TRISG13 = 0;
#endif
#ifdef _ANSG13
  _ANSG13 = 0;  //disable analog
#endif
}
#ifdef _ODCG13
#define ENABLE_RG13_OPENDRAIN() _ODCG13 = 1
#define DISABLE_RG13_OPENDRAIN() _ODCG13 = 0
#define CONFIG_RG13_AS_DIG_OD_OUTPUT()  {CONFIG_RG13_AS_DIG_OUTPUT();ENABLE_RG13_OPENDRAIN();}
#endif
static inline void CONFIG_RG13_AS_DIG_INPUT() {
#ifdef _CNPUG13
  _CNPUG13 = 0; //disable pullup
#endif
#ifdef _CNPDG13
  _CNPDG13 = 0; //disable pulldown
#endif
#ifdef _TRISG13
  _TRISG13 = 1;
#endif
#ifdef _ANSG13
  _ANSG13 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG13
#ifdef _RG14
#ifdef _CNPUG14
#define ENABLE_RG14_PULLUP() _CNPUG14 = 1
#define DISABLE_RG14_PULLUP() _CNPUG14 = 0
#endif
#ifdef _CNPDG14
#define ENABLE_RG14_PULLDOWN() _CNPDG14 = 1
#define DISABLE_RG14_PULLDOWN() _CNPDG14 = 0
#endif
#ifdef _ANSG14
#define ENABLE_RG14_ANALOG() _ANSG14 = 1
#define DISABLE_RG14_ANALOG() _ANSG14 = 0
#endif
#ifdef _CNIEG14
#define ENABLE_RG14_CN_INTERRUPT() _CNIEG14 = 1
#define DISABLE_RG14_CN_INTERRUPT() _CNIEG14 = 0
#endif
static inline void CONFIG_RG14_AS_DIG_OUTPUT() {
#ifdef _CNPUG14
  _CNPUG14 = 0; //disable pullup
#endif
#ifdef _CNPDG14
  _CNPDG14 = 0; //disable pulldown
#endif
#ifdef _ODCG14
  _ODCG14 = 0;
#endif
#ifdef _TRISG14
  _TRISG14 = 0;
#endif
#ifdef _ANSG14
  _ANSG14 = 0;  //disable analog
#endif
}
#ifdef _ODCG14
#define ENABLE_RG14_OPENDRAIN() _ODCG14 = 1
#define DISABLE_RG14_OPENDRAIN() _ODCG14 = 0
#define CONFIG_RG14_AS_DIG_OD_OUTPUT()  {CONFIG_RG14_AS_DIG_OUTPUT();ENABLE_RG14_OPENDRAIN();}
#endif
static inline void CONFIG_RG14_AS_DIG_INPUT() {
#ifdef _CNPUG14
  _CNPUG14 = 0; //disable pullup
#endif
#ifdef _CNPDG14
  _CNPDG14 = 0; //disable pulldown
#endif
#ifdef _TRISG14
  _TRISG14 = 1;
#endif
#ifdef _ANSG14
  _ANSG14 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG14
#ifdef _RG15
#ifdef _CNPUG15
#define ENABLE_RG15_PULLUP() _CNPUG15 = 1
#define DISABLE_RG15_PULLUP() _CNPUG15 = 0
#endif
#ifdef _CNPDG15
#define ENABLE_RG15_PULLDOWN() _CNPDG15 = 1
#define DISABLE_RG15_PULLDOWN() _CNPDG15 = 0
#endif
#ifdef _ANSG15
#define ENABLE_RG15_ANALOG() _ANSG15 = 1
#define DISABLE_RG15_ANALOG() _ANSG15 = 0
#endif
#ifdef _CNIEG15
#define ENABLE_RG15_CN_INTERRUPT() _CNIEG15 = 1
#define DISABLE_RG15_CN_INTERRUPT() _CNIEG15 = 0
#endif
static inline void CONFIG_RG15_AS_DIG_OUTPUT() {
#ifdef _CNPUG15
  _CNPUG15 = 0; //disable pullup
#endif
#ifdef _CNPDG15
  _CNPDG15 = 0; //disable pulldown
#endif
#ifdef _ODCG15
  _ODCG15 = 0;
#endif
#ifdef _TRISG15
  _TRISG15 = 0;
#endif
#ifdef _ANSG15
  _ANSG15 = 0;  //disable analog
#endif
}
#ifdef _ODCG15
#define ENABLE_RG15_OPENDRAIN() _ODCG15 = 1
#define DISABLE_RG15_OPENDRAIN() _ODCG15 = 0
#define CONFIG_RG15_AS_DIG_OD_OUTPUT()  {CONFIG_RG15_AS_DIG_OUTPUT();ENABLE_RG15_OPENDRAIN();}
#endif
static inline void CONFIG_RG15_AS_DIG_INPUT() {
#ifdef _CNPUG15
  _CNPUG15 = 0; //disable pullup
#endif
#ifdef _CNPDG15
  _CNPDG15 = 0; //disable pulldown
#endif
#ifdef _TRISG15
  _TRISG15 = 1;
#endif
#ifdef _ANSG15
  _ANSG15 = 0;  //disable analog
#endif
}
#endif  //endif #ifdef _RG15
#define CONFIG_RP0_AS_DIG_PIN()
#define CONFIG_RP1_AS_DIG_PIN()
#define CONFIG_RP2_AS_DIG_PIN()
#define CONFIG_RP3_AS_DIG_PIN()
#define CONFIG_RP4_AS_DIG_PIN()
#define CONFIG_RP5_AS_DIG_PIN()
#define CONFIG_RP6_AS_DIG_PIN()
#define CONFIG_RP7_AS_DIG_PIN()
#define CONFIG_RP8_AS_DIG_PIN()
#define CONFIG_RP9_AS_DIG_PIN()
#define CONFIG_RP10_AS_DIG_PIN()
#define CONFIG_RP11_AS_DIG_PIN()
#define CONFIG_RP12_AS_DIG_PIN()
#define CONFIG_RP13_AS_DIG_PIN()
#define CONFIG_RP14_AS_DIG_PIN()
#define CONFIG_RP15_AS_DIG_PIN()
#define CONFIG_RP16_AS_DIG_PIN()
#define CONFIG_RP17_AS_DIG_PIN()
#define CONFIG_RP18_AS_DIG_PIN()
#define CONFIG_RP19_AS_DIG_PIN()
#define CONFIG_RP20_AS_DIG_PIN()
#define CONFIG_RP21_AS_DIG_PIN()
#define CONFIG_RP22_AS_DIG_PIN()
#define CONFIG_RP23_AS_DIG_PIN()
#define CONFIG_RP24_AS_DIG_PIN()
#define CONFIG_RP25_AS_DIG_PIN()
#define CONFIG_RP26_AS_DIG_PIN()
#define CONFIG_RP27_AS_DIG_PIN()
#define CONFIG_RP28_AS_DIG_PIN()
#define CONFIG_RP29_AS_DIG_PIN()
#define CONFIG_RP30_AS_DIG_PIN()
#define CONFIG_RP31_AS_DIG_PIN()
#define CONFIG_RP32_AS_DIG_PIN()  _ANSB0 = 1
#define CONFIG_RP33_AS_DIG_PIN()  _ANSB1 = 1
#define CONFIG_RP34_AS_DIG_PIN()  _ANSB2 = 1
#define CONFIG_RP35_AS_DIG_PIN()  _ANSB3 = 1
#define CONFIG_RP36_AS_DIG_PIN()  _ANSB4 = 1
#define CONFIG_RP37_AS_DIG_PIN()  _ANSB5 = 1
#define CONFIG_RP38_AS_DIG_PIN()  _ANSB6 = 1
#define CONFIG_RP39_AS_DIG_PIN()  _ANSB7 = 1
#define CONFIG_RP40_AS_DIG_PIN()  _ANSB8 = 1
#define CONFIG_RP41_AS_DIG_PIN()  _ANSB9 = 1
#define CONFIG_RP42_AS_DIG_PIN()  _ANSB10 = 1
#define CONFIG_RP43_AS_DIG_PIN()  _ANSB11 = 1
#define CONFIG_RP44_AS_DIG_PIN()  _ANSB12 = 1
#define CONFIG_RP45_AS_DIG_PIN()  _ANSB13 = 1
#define CONFIG_RP46_AS_DIG_PIN()  _ANSB14 = 1
#define CONFIG_RP47_AS_DIG_PIN()  _ANSB15 = 1
#define CONFIG_RP48_AS_DIG_PIN()
#define CONFIG_RP49_AS_DIG_PIN()
#define CONFIG_RP50_AS_DIG_PIN()
#define CONFIG_RP51_AS_DIG_PIN()
#define CONFIG_RP52_AS_DIG_PIN()
#define CONFIG_RP53_AS_DIG_PIN()
#define CONFIG_RP54_AS_DIG_PIN()
#define CONFIG_RP55_AS_DIG_PIN()
#define CONFIG_RP56_AS_DIG_PIN()
#define CONFIG_RP57_AS_DIG_PIN()
#define CONFIG_RP58_AS_DIG_PIN()
#define CONFIG_RP59_AS_DIG_PIN()
#define CONFIG_RP60_AS_DIG_PIN()
#define CONFIG_RP61_AS_DIG_PIN()
#define CONFIG_RP62_AS_DIG_PIN()
#define CONFIG_RP63_AS_DIG_PIN()
#define CONFIG_RP64_AS_DIG_PIN()
#define CONFIG_RP65_AS_DIG_PIN()
#define CONFIG_RP66_AS_DIG_PIN()
#define CONFIG_RP67_AS_DIG_PIN()
#define CONFIG_RP68_AS_DIG_PIN()
#define CONFIG_RP69_AS_DIG_PIN()
#define CONFIG_RP70_AS_DIG_PIN()
#define CONFIG_RP71_AS_DIG_PIN()
#define CONFIG_RP72_AS_DIG_PIN()
#define CONFIG_RP73_AS_DIG_PIN()
#define CONFIG_RP74_AS_DIG_PIN()
#define CONFIG_RP75_AS_DIG_PIN()
#define CONFIG_RP76_AS_DIG_PIN()
#define CONFIG_RP77_AS_DIG_PIN()
#define CONFIG_RP78_AS_DIG_PIN()
#define CONFIG_RP79_AS_DIG_PIN()
#define CONFIG_RP80_AS_DIG_PIN()  _ANSE0 = 1
#define CONFIG_RP81_AS_DIG_PIN()  _ANSE1 = 1
#define CONFIG_RP82_AS_DIG_PIN()  _ANSE2 = 1
#define CONFIG_RP83_AS_DIG_PIN()  _ANSE3 = 1
#define CONFIG_RP84_AS_DIG_PIN()  _ANSE4 = 1
#define CONFIG_RP85_AS_DIG_PIN()  _ANSE5 = 1
#define CONFIG_RP86_AS_DIG_PIN()  _ANSE6 = 1
#define CONFIG_RP87_AS_DIG_PIN()  _ANSE7 = 1
#define CONFIG_RP88_AS_DIG_PIN()
#define CONFIG_RP89_AS_DIG_PIN()
#define CONFIG_RP90_AS_DIG_PIN()
#define CONFIG_RP91_AS_DIG_PIN()
#define CONFIG_RP92_AS_DIG_PIN()
#define CONFIG_RP93_AS_DIG_PIN()
#define CONFIG_RP94_AS_DIG_PIN()
#define CONFIG_RP95_AS_DIG_PIN()
#define CONFIG_RP96_AS_DIG_PIN()
#define CONFIG_RP97_AS_DIG_PIN()
#define CONFIG_RP98_AS_DIG_PIN()
#define CONFIG_RP99_AS_DIG_PIN()
#define DISABLE_C2IND_ANALOG()
#define DISABLE_U2CTS_ANALOG()
#define DISABLE_CTPLS_ANALOG()
#define DISABLE_U4TX_ANALOG()
#define DISABLE_SS2_ANALOG()
#define DISABLE_U3TX_ANALOG()
#define DISABLE_C1OUT_ANALOG()
#define DISABLE_INT1_ANALOG()
#define DISABLE_U1RTS_ANALOG()
#define DISABLE_T5CK_ANALOG()
#define DISABLE_C1IND_ANALOG()
#define DISABLE_U3CTS_ANALOG()
#define DISABLE_OC5_ANALOG()
#define DISABLE_U2TX_ANALOG()
#define DISABLE_C2INA_ANALOG()
#define DISABLE_INT0_ANALOG()
#define DISABLE_U1BCLK_ANALOG()
#define DISABLE_SS1_ANALOG()
#define DISABLE_IC4_ANALOG()
#define DISABLE_OCFB_ANALOG()
#define DISABLE_C1TX_ANALOG()
#define DISABLE_RTCC_ANALOG()
#define DISABLE_IC3_ANALOG()
#define DISABLE_T3CK_ANALOG()
#define DISABLE_C2OUT_ANALOG()
#define DISABLE_INT2_ANALOG()
#define DISABLE_OCFA_ANALOG()
#define DISABLE_OC4_ANALOG()
#define DISABLE_U1RX_ANALOG()
#define DISABLE_IC8_ANALOG()
#define DISABLE_U3RX_ANALOG()
#define DISABLE_CTED2_ANALOG()
#define DISABLE_IC1_ANALOG()
#define DISABLE_U2BCLK_ANALOG()
#define DISABLE_C1INA_ANALOG()
#define DISABLE_T4CK_ANALOG()
#define DISABLE_C2INC_ANALOG()
#define DISABLE_IC7_ANALOG()
#define DISABLE_IC2_ANALOG()
#define DISABLE_U2RTS_ANALOG()
#define DISABLE_U4RTS_ANALOG()
#define DISABLE_SDI2_ANALOG()
#define DISABLE_OC2_ANALOG()
#define DISABLE_C1RX_ANALOG()
#define DISABLE_C2RX_ANALOG()
#define DISABLE_SDO1_ANALOG()
#define DISABLE_U1CTS_ANALOG()
#define DISABLE_OC3_ANALOG()
#define DISABLE_C21INB_ANALOG()
#define DISABLE_U1TX_ANALOG()
#define DISABLE_T1CK_ANALOG()
#define DISABLE_SCK1_ANALOG()
#define DISABLE_U2RX_ANALOG()
#define DISABLE_T2CK_ANALOG()
#define DISABLE_CTED1_ANALOG()
#define DISABLE_U3RTS_ANALOG()
#define DISABLE_SDO2_ANALOG()
#define DISABLE_C1INC_ANALOG()
#define DISABLE_U4RX_ANALOG()
#define DISABLE_IC5_ANALOG()
#define DISABLE_SDI1_ANALOG()
#define DISABLE_C1INB_ANALOG()
#define DISABLE_OC1_ANALOG()
#define DISABLE_SCK2_ANALOG()
#define DISABLE_IC6_ANALOG()
#define DISABLE_U4CTS_ANALOG()
#define _PIC24_DIGIO_DEFINED
