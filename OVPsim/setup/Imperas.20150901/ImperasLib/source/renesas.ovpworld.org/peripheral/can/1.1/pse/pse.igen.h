/*
 * Copyright (c) 2005-2015 Imperas Software Ltd., www.imperas.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150901.0
//
////////////////////////////////////////////////////////////////////////////////

#ifndef PSE_IGEN_H
#define PSE_IGEN_H

#ifdef _PSE_
#    include "peripheral/impTypes.h"
#    include "peripheral/bhm.h"
#    include "peripheral/ppm.h"
#else
#    include "hostapi/impTypes.h"
#endif

//////////////////////////////////// Externs ///////////////////////////////////

extern Uns32 diagnosticLevel;


/////////////////////////// Dynamic Diagnostic Macros //////////////////////////

// Bottom two bits of word used for PSE diagnostics
#define PSE_DIAG_LOW      (BHM_DIAG_MASK_LOW(diagnosticLevel))
#define PSE_DIAG_MEDIUM   (BHM_DIAG_MASK_MEDIUM(diagnosticLevel))
#define PSE_DIAG_HIGH     (BHM_DIAG_MASK_HIGH(diagnosticLevel))
// Next two bits of word used for PSE semihost/intercept library diagnostics
#define PSE_DIAG_SEMIHOST (BHM_DIAG_MASK_SEMIHOST(diagnosticLevel))

/////////////////////////// Register data declaration //////////////////////////

typedef struct busPortMR_MR_dataS { 
    union { 
        Uns8 value;
    } GMCS;
    union { 
        Uns8 value;
    } GMABTD;
    union { 
        Uns8 value;
    } LEC;
    union { 
        Uns8 value;
    } INFO;
    union { 
        Uns8 value;
    } BRP;
    union { 
        Uns8 value;
    } LIPT;
    union { 
        Uns8 value;
    } LOPT;
} busPortMR_MR_dataT, *busPortMR_MR_dataTP;

typedef struct busPortMDB_MDB_dataS { 
    union { 
        Uns32 value;
    } MDATA000;
    union { 
        Uns32 value;
    } MDATA100;
    union { 
        Uns32 value;
    } MDATA200;
    union { 
        Uns32 value;
    } MDATA300;
    union { 
        Uns32 value;
    } MDATA400;
    union { 
        Uns32 value;
    } MDATA500;
    union { 
        Uns32 value;
    } MDATA600;
    union { 
        Uns32 value;
    } MDATA700;
    union { 
        Uns32 value;
    } MDLC00;
    union { 
        Uns32 value;
    } MCONF00;
    union { 
        Uns32 value;
    } MDATA001;
    union { 
        Uns32 value;
    } MDATA101;
    union { 
        Uns32 value;
    } MDATA201;
    union { 
        Uns32 value;
    } MDATA301;
    union { 
        Uns32 value;
    } MDATA401;
    union { 
        Uns32 value;
    } MDATA501;
    union { 
        Uns32 value;
    } MDATA601;
    union { 
        Uns32 value;
    } MDATA701;
    union { 
        Uns32 value;
    } MDLC01;
    union { 
        Uns32 value;
    } MCONF01;
    union { 
        Uns32 value;
    } MDATA002;
    union { 
        Uns32 value;
    } MDATA102;
    union { 
        Uns32 value;
    } MDATA202;
    union { 
        Uns32 value;
    } MDATA302;
    union { 
        Uns32 value;
    } MDATA402;
    union { 
        Uns32 value;
    } MDATA502;
    union { 
        Uns32 value;
    } MDATA602;
    union { 
        Uns32 value;
    } MDATA702;
    union { 
        Uns32 value;
    } MDLC02;
    union { 
        Uns32 value;
    } MCONF02;
    union { 
        Uns32 value;
    } MDATA003;
    union { 
        Uns32 value;
    } MDATA103;
    union { 
        Uns32 value;
    } MDATA203;
    union { 
        Uns32 value;
    } MDATA303;
    union { 
        Uns32 value;
    } MDATA403;
    union { 
        Uns32 value;
    } MDATA503;
    union { 
        Uns32 value;
    } MDATA603;
    union { 
        Uns32 value;
    } MDATA703;
    union { 
        Uns32 value;
    } MDLC03;
    union { 
        Uns32 value;
    } MCONF03;
    union { 
        Uns32 value;
    } MDATA004;
    union { 
        Uns32 value;
    } MDATA104;
    union { 
        Uns32 value;
    } MDATA204;
    union { 
        Uns32 value;
    } MDATA304;
    union { 
        Uns32 value;
    } MDATA404;
    union { 
        Uns32 value;
    } MDATA504;
    union { 
        Uns32 value;
    } MDATA604;
    union { 
        Uns32 value;
    } MDATA704;
    union { 
        Uns32 value;
    } MDLC04;
    union { 
        Uns32 value;
    } MCONF04;
    union { 
        Uns32 value;
    } MDATA005;
    union { 
        Uns32 value;
    } MDATA105;
    union { 
        Uns32 value;
    } MDATA205;
    union { 
        Uns32 value;
    } MDATA305;
    union { 
        Uns32 value;
    } MDATA405;
    union { 
        Uns32 value;
    } MDATA505;
    union { 
        Uns32 value;
    } MDATA605;
    union { 
        Uns32 value;
    } MDATA705;
    union { 
        Uns32 value;
    } MDLC05;
    union { 
        Uns32 value;
    } MCONF05;
    union { 
        Uns32 value;
    } MDATA006;
    union { 
        Uns32 value;
    } MDATA106;
    union { 
        Uns32 value;
    } MDATA206;
    union { 
        Uns32 value;
    } MDATA306;
    union { 
        Uns32 value;
    } MDATA406;
    union { 
        Uns32 value;
    } MDATA506;
    union { 
        Uns32 value;
    } MDATA606;
    union { 
        Uns32 value;
    } MDATA706;
    union { 
        Uns32 value;
    } MDLC06;
    union { 
        Uns32 value;
    } MCONF06;
    union { 
        Uns32 value;
    } MDATA007;
    union { 
        Uns32 value;
    } MDATA107;
    union { 
        Uns32 value;
    } MDATA207;
    union { 
        Uns32 value;
    } MDATA307;
    union { 
        Uns32 value;
    } MDATA407;
    union { 
        Uns32 value;
    } MDATA507;
    union { 
        Uns32 value;
    } MDATA607;
    union { 
        Uns32 value;
    } MDATA707;
    union { 
        Uns32 value;
    } MDLC07;
    union { 
        Uns32 value;
    } MCONF07;
    union { 
        Uns32 value;
    } MDATA008;
    union { 
        Uns32 value;
    } MDATA108;
    union { 
        Uns32 value;
    } MDATA208;
    union { 
        Uns32 value;
    } MDATA308;
    union { 
        Uns32 value;
    } MDATA408;
    union { 
        Uns32 value;
    } MDATA508;
    union { 
        Uns32 value;
    } MDATA608;
    union { 
        Uns32 value;
    } MDATA708;
    union { 
        Uns32 value;
    } MDLC08;
    union { 
        Uns32 value;
    } MCONF08;
    union { 
        Uns32 value;
    } MDATA009;
    union { 
        Uns32 value;
    } MDATA109;
    union { 
        Uns32 value;
    } MDATA209;
    union { 
        Uns32 value;
    } MDATA309;
    union { 
        Uns32 value;
    } MDATA409;
    union { 
        Uns32 value;
    } MDATA509;
    union { 
        Uns32 value;
    } MDATA609;
    union { 
        Uns32 value;
    } MDATA709;
    union { 
        Uns32 value;
    } MDLC09;
    union { 
        Uns32 value;
    } MCONF09;
    union { 
        Uns32 value;
    } MDATA010;
    union { 
        Uns32 value;
    } MDATA110;
    union { 
        Uns32 value;
    } MDATA210;
    union { 
        Uns32 value;
    } MDATA310;
    union { 
        Uns32 value;
    } MDATA410;
    union { 
        Uns32 value;
    } MDATA510;
    union { 
        Uns32 value;
    } MDATA610;
    union { 
        Uns32 value;
    } MDATA710;
    union { 
        Uns32 value;
    } MDLC10;
    union { 
        Uns32 value;
    } MCONF10;
    union { 
        Uns32 value;
    } MDATA011;
    union { 
        Uns32 value;
    } MDATA111;
    union { 
        Uns32 value;
    } MDATA211;
    union { 
        Uns32 value;
    } MDATA311;
    union { 
        Uns32 value;
    } MDATA411;
    union { 
        Uns32 value;
    } MDATA511;
    union { 
        Uns32 value;
    } MDATA611;
    union { 
        Uns32 value;
    } MDATA711;
    union { 
        Uns32 value;
    } MDLC11;
    union { 
        Uns32 value;
    } MCONF11;
    union { 
        Uns32 value;
    } MDATA012;
    union { 
        Uns32 value;
    } MDATA112;
    union { 
        Uns32 value;
    } MDATA212;
    union { 
        Uns32 value;
    } MDATA312;
    union { 
        Uns32 value;
    } MDATA412;
    union { 
        Uns32 value;
    } MDATA512;
    union { 
        Uns32 value;
    } MDATA612;
    union { 
        Uns32 value;
    } MDATA712;
    union { 
        Uns32 value;
    } MDLC12;
    union { 
        Uns32 value;
    } MCONF12;
    union { 
        Uns32 value;
    } MDATA013;
    union { 
        Uns32 value;
    } MDATA113;
    union { 
        Uns32 value;
    } MDATA213;
    union { 
        Uns32 value;
    } MDATA313;
    union { 
        Uns32 value;
    } MDATA413;
    union { 
        Uns32 value;
    } MDATA513;
    union { 
        Uns32 value;
    } MDATA613;
    union { 
        Uns32 value;
    } MDATA713;
    union { 
        Uns32 value;
    } MDLC13;
    union { 
        Uns32 value;
    } MCONF13;
    union { 
        Uns32 value;
    } MDATA014;
    union { 
        Uns32 value;
    } MDATA114;
    union { 
        Uns32 value;
    } MDATA214;
    union { 
        Uns32 value;
    } MDATA314;
    union { 
        Uns32 value;
    } MDATA414;
    union { 
        Uns32 value;
    } MDATA514;
    union { 
        Uns32 value;
    } MDATA614;
    union { 
        Uns32 value;
    } MDATA714;
    union { 
        Uns32 value;
    } MDLC14;
    union { 
        Uns32 value;
    } MCONF14;
    union { 
        Uns32 value;
    } MDATA015;
    union { 
        Uns32 value;
    } MDATA115;
    union { 
        Uns32 value;
    } MDATA215;
    union { 
        Uns32 value;
    } MDATA315;
    union { 
        Uns32 value;
    } MDATA415;
    union { 
        Uns32 value;
    } MDATA515;
    union { 
        Uns32 value;
    } MDATA615;
    union { 
        Uns32 value;
    } MDATA715;
    union { 
        Uns32 value;
    } MDLC15;
    union { 
        Uns32 value;
    } MCONF15;
    union { 
        Uns32 value;
    } MDATA016;
    union { 
        Uns32 value;
    } MDATA116;
    union { 
        Uns32 value;
    } MDATA216;
    union { 
        Uns32 value;
    } MDATA316;
    union { 
        Uns32 value;
    } MDATA416;
    union { 
        Uns32 value;
    } MDATA516;
    union { 
        Uns32 value;
    } MDATA616;
    union { 
        Uns32 value;
    } MDATA716;
    union { 
        Uns32 value;
    } MDLC16;
    union { 
        Uns32 value;
    } MCONF16;
    union { 
        Uns32 value;
    } MDATA017;
    union { 
        Uns32 value;
    } MDATA117;
    union { 
        Uns32 value;
    } MDATA217;
    union { 
        Uns32 value;
    } MDATA317;
    union { 
        Uns32 value;
    } MDATA417;
    union { 
        Uns32 value;
    } MDATA517;
    union { 
        Uns32 value;
    } MDATA617;
    union { 
        Uns32 value;
    } MDATA717;
    union { 
        Uns32 value;
    } MDLC17;
    union { 
        Uns32 value;
    } MCONF17;
    union { 
        Uns32 value;
    } MDATA018;
    union { 
        Uns32 value;
    } MDATA118;
    union { 
        Uns32 value;
    } MDATA218;
    union { 
        Uns32 value;
    } MDATA318;
    union { 
        Uns32 value;
    } MDATA418;
    union { 
        Uns32 value;
    } MDATA518;
    union { 
        Uns32 value;
    } MDATA618;
    union { 
        Uns32 value;
    } MDATA718;
    union { 
        Uns32 value;
    } MDLC18;
    union { 
        Uns32 value;
    } MCONF18;
    union { 
        Uns32 value;
    } MDATA019;
    union { 
        Uns32 value;
    } MDATA119;
    union { 
        Uns32 value;
    } MDATA219;
    union { 
        Uns32 value;
    } MDATA319;
    union { 
        Uns32 value;
    } MDATA419;
    union { 
        Uns32 value;
    } MDATA519;
    union { 
        Uns32 value;
    } MDATA619;
    union { 
        Uns32 value;
    } MDATA719;
    union { 
        Uns32 value;
    } MDLC19;
    union { 
        Uns32 value;
    } MCONF19;
    union { 
        Uns32 value;
    } MDATA020;
    union { 
        Uns32 value;
    } MDATA120;
    union { 
        Uns32 value;
    } MDATA220;
    union { 
        Uns32 value;
    } MDATA320;
    union { 
        Uns32 value;
    } MDATA420;
    union { 
        Uns32 value;
    } MDATA520;
    union { 
        Uns32 value;
    } MDATA620;
    union { 
        Uns32 value;
    } MDATA720;
    union { 
        Uns32 value;
    } MDLC20;
    union { 
        Uns32 value;
    } MCONF20;
    union { 
        Uns32 value;
    } MDATA021;
    union { 
        Uns32 value;
    } MDATA121;
    union { 
        Uns32 value;
    } MDATA221;
    union { 
        Uns32 value;
    } MDATA321;
    union { 
        Uns32 value;
    } MDATA421;
    union { 
        Uns32 value;
    } MDATA521;
    union { 
        Uns32 value;
    } MDATA621;
    union { 
        Uns32 value;
    } MDATA721;
    union { 
        Uns32 value;
    } MDLC21;
    union { 
        Uns32 value;
    } MCONF21;
    union { 
        Uns32 value;
    } MDATA022;
    union { 
        Uns32 value;
    } MDATA122;
    union { 
        Uns32 value;
    } MDATA222;
    union { 
        Uns32 value;
    } MDATA322;
    union { 
        Uns32 value;
    } MDATA422;
    union { 
        Uns32 value;
    } MDATA522;
    union { 
        Uns32 value;
    } MDATA622;
    union { 
        Uns32 value;
    } MDATA722;
    union { 
        Uns32 value;
    } MDLC22;
    union { 
        Uns32 value;
    } MCONF22;
    union { 
        Uns32 value;
    } MDATA023;
    union { 
        Uns32 value;
    } MDATA123;
    union { 
        Uns32 value;
    } MDATA223;
    union { 
        Uns32 value;
    } MDATA323;
    union { 
        Uns32 value;
    } MDATA423;
    union { 
        Uns32 value;
    } MDATA523;
    union { 
        Uns32 value;
    } MDATA623;
    union { 
        Uns32 value;
    } MDATA723;
    union { 
        Uns32 value;
    } MDLC23;
    union { 
        Uns32 value;
    } MCONF23;
    union { 
        Uns32 value;
    } MDATA024;
    union { 
        Uns32 value;
    } MDATA124;
    union { 
        Uns32 value;
    } MDATA224;
    union { 
        Uns32 value;
    } MDATA324;
    union { 
        Uns32 value;
    } MDATA424;
    union { 
        Uns32 value;
    } MDATA524;
    union { 
        Uns32 value;
    } MDATA624;
    union { 
        Uns32 value;
    } MDATA724;
    union { 
        Uns32 value;
    } MDLC24;
    union { 
        Uns32 value;
    } MCONF24;
    union { 
        Uns32 value;
    } MDATA025;
    union { 
        Uns32 value;
    } MDATA125;
    union { 
        Uns32 value;
    } MDATA225;
    union { 
        Uns32 value;
    } MDATA325;
    union { 
        Uns32 value;
    } MDATA425;
    union { 
        Uns32 value;
    } MDATA525;
    union { 
        Uns32 value;
    } MDATA625;
    union { 
        Uns32 value;
    } MDATA725;
    union { 
        Uns32 value;
    } MDLC25;
    union { 
        Uns32 value;
    } MCONF25;
    union { 
        Uns32 value;
    } MDATA026;
    union { 
        Uns32 value;
    } MDATA126;
    union { 
        Uns32 value;
    } MDATA226;
    union { 
        Uns32 value;
    } MDATA326;
    union { 
        Uns32 value;
    } MDATA426;
    union { 
        Uns32 value;
    } MDATA526;
    union { 
        Uns32 value;
    } MDATA626;
    union { 
        Uns32 value;
    } MDATA726;
    union { 
        Uns32 value;
    } MDLC26;
    union { 
        Uns32 value;
    } MCONF26;
    union { 
        Uns32 value;
    } MDATA027;
    union { 
        Uns32 value;
    } MDATA127;
    union { 
        Uns32 value;
    } MDATA227;
    union { 
        Uns32 value;
    } MDATA327;
    union { 
        Uns32 value;
    } MDATA427;
    union { 
        Uns32 value;
    } MDATA527;
    union { 
        Uns32 value;
    } MDATA627;
    union { 
        Uns32 value;
    } MDATA727;
    union { 
        Uns32 value;
    } MDLC27;
    union { 
        Uns32 value;
    } MCONF27;
    union { 
        Uns32 value;
    } MDATA028;
    union { 
        Uns32 value;
    } MDATA128;
    union { 
        Uns32 value;
    } MDATA228;
    union { 
        Uns32 value;
    } MDATA328;
    union { 
        Uns32 value;
    } MDATA428;
    union { 
        Uns32 value;
    } MDATA528;
    union { 
        Uns32 value;
    } MDATA628;
    union { 
        Uns32 value;
    } MDATA728;
    union { 
        Uns32 value;
    } MDLC28;
    union { 
        Uns32 value;
    } MCONF28;
    union { 
        Uns32 value;
    } MDATA029;
    union { 
        Uns32 value;
    } MDATA129;
    union { 
        Uns32 value;
    } MDATA229;
    union { 
        Uns32 value;
    } MDATA329;
    union { 
        Uns32 value;
    } MDATA429;
    union { 
        Uns32 value;
    } MDATA529;
    union { 
        Uns32 value;
    } MDATA629;
    union { 
        Uns32 value;
    } MDATA729;
    union { 
        Uns32 value;
    } MDLC29;
    union { 
        Uns32 value;
    } MCONF29;
    union { 
        Uns32 value;
    } MDATA030;
    union { 
        Uns32 value;
    } MDATA130;
    union { 
        Uns32 value;
    } MDATA230;
    union { 
        Uns32 value;
    } MDATA330;
    union { 
        Uns32 value;
    } MDATA430;
    union { 
        Uns32 value;
    } MDATA530;
    union { 
        Uns32 value;
    } MDATA630;
    union { 
        Uns32 value;
    } MDATA730;
    union { 
        Uns32 value;
    } MDLC30;
    union { 
        Uns32 value;
    } MCONF30;
    union { 
        Uns32 value;
    } MDATA031;
    union { 
        Uns32 value;
    } MDATA131;
    union { 
        Uns32 value;
    } MDATA231;
    union { 
        Uns32 value;
    } MDATA331;
    union { 
        Uns32 value;
    } MDATA431;
    union { 
        Uns32 value;
    } MDATA531;
    union { 
        Uns32 value;
    } MDATA631;
    union { 
        Uns32 value;
    } MDATA731;
    union { 
        Uns32 value;
    } MDLC31;
    union { 
        Uns32 value;
    } MCONF31;
} busPortMDB_MDB_dataT, *busPortMDB_MDB_dataTP;

typedef struct busPortMDH_MDH_dataS { 
    union { 
        Uns32 value;
    } MDATA0100;
    union { 
        Uns32 value;
    } MDATA2300;
    union { 
        Uns32 value;
    } MDATA4500;
    union { 
        Uns32 value;
    } MDATA6700;
    union { 
        Uns16 value;
    } MIDL00;
    union { 
        Uns16 value;
    } MIDH00;
    union { 
        Uns16 value;
    } MCTRL00;
    union { 
        Uns32 value;
    } MDATA0101;
    union { 
        Uns32 value;
    } MDATA2301;
    union { 
        Uns32 value;
    } MDATA4501;
    union { 
        Uns32 value;
    } MDATA6701;
    union { 
        Uns16 value;
    } MIDL01;
    union { 
        Uns16 value;
    } MIDH01;
    union { 
        Uns16 value;
    } MCTRL01;
    union { 
        Uns32 value;
    } MDATA0102;
    union { 
        Uns32 value;
    } MDATA2302;
    union { 
        Uns32 value;
    } MDATA4502;
    union { 
        Uns32 value;
    } MDATA6702;
    union { 
        Uns16 value;
    } MIDL02;
    union { 
        Uns16 value;
    } MIDH02;
    union { 
        Uns16 value;
    } MCTRL02;
    union { 
        Uns32 value;
    } MDATA0103;
    union { 
        Uns32 value;
    } MDATA2303;
    union { 
        Uns32 value;
    } MDATA4503;
    union { 
        Uns32 value;
    } MDATA6703;
    union { 
        Uns16 value;
    } MIDL03;
    union { 
        Uns16 value;
    } MIDH03;
    union { 
        Uns16 value;
    } MCTRL03;
    union { 
        Uns32 value;
    } MDATA0104;
    union { 
        Uns32 value;
    } MDATA2304;
    union { 
        Uns32 value;
    } MDATA4504;
    union { 
        Uns32 value;
    } MDATA6704;
    union { 
        Uns16 value;
    } MIDL04;
    union { 
        Uns16 value;
    } MIDH04;
    union { 
        Uns16 value;
    } MCTRL04;
    union { 
        Uns32 value;
    } MDATA0105;
    union { 
        Uns32 value;
    } MDATA2305;
    union { 
        Uns32 value;
    } MDATA4505;
    union { 
        Uns32 value;
    } MDATA6705;
    union { 
        Uns16 value;
    } MIDL05;
    union { 
        Uns16 value;
    } MIDH05;
    union { 
        Uns16 value;
    } MCTRL05;
    union { 
        Uns32 value;
    } MDATA0106;
    union { 
        Uns32 value;
    } MDATA2306;
    union { 
        Uns32 value;
    } MDATA4506;
    union { 
        Uns32 value;
    } MDATA6706;
    union { 
        Uns16 value;
    } MIDL06;
    union { 
        Uns16 value;
    } MIDH06;
    union { 
        Uns16 value;
    } MCTRL06;
    union { 
        Uns32 value;
    } MDATA0107;
    union { 
        Uns32 value;
    } MDATA2307;
    union { 
        Uns32 value;
    } MDATA4507;
    union { 
        Uns32 value;
    } MDATA6707;
    union { 
        Uns16 value;
    } MIDL07;
    union { 
        Uns16 value;
    } MIDH07;
    union { 
        Uns16 value;
    } MCTRL07;
    union { 
        Uns32 value;
    } MDATA0108;
    union { 
        Uns32 value;
    } MDATA2308;
    union { 
        Uns32 value;
    } MDATA4508;
    union { 
        Uns32 value;
    } MDATA6708;
    union { 
        Uns16 value;
    } MIDL08;
    union { 
        Uns16 value;
    } MIDH08;
    union { 
        Uns16 value;
    } MCTRL08;
    union { 
        Uns32 value;
    } MDATA0109;
    union { 
        Uns32 value;
    } MDATA2309;
    union { 
        Uns32 value;
    } MDATA4509;
    union { 
        Uns32 value;
    } MDATA6709;
    union { 
        Uns16 value;
    } MIDL09;
    union { 
        Uns16 value;
    } MIDH09;
    union { 
        Uns16 value;
    } MCTRL09;
    union { 
        Uns32 value;
    } MDATA0110;
    union { 
        Uns32 value;
    } MDATA2310;
    union { 
        Uns32 value;
    } MDATA4510;
    union { 
        Uns32 value;
    } MDATA6710;
    union { 
        Uns16 value;
    } MIDL10;
    union { 
        Uns16 value;
    } MIDH10;
    union { 
        Uns16 value;
    } MCTRL10;
    union { 
        Uns32 value;
    } MDATA0111;
    union { 
        Uns32 value;
    } MDATA2311;
    union { 
        Uns32 value;
    } MDATA4511;
    union { 
        Uns32 value;
    } MDATA6711;
    union { 
        Uns16 value;
    } MIDL11;
    union { 
        Uns16 value;
    } MIDH11;
    union { 
        Uns16 value;
    } MCTRL11;
    union { 
        Uns32 value;
    } MDATA0112;
    union { 
        Uns32 value;
    } MDATA2312;
    union { 
        Uns32 value;
    } MDATA4512;
    union { 
        Uns32 value;
    } MDATA6712;
    union { 
        Uns16 value;
    } MIDL12;
    union { 
        Uns16 value;
    } MIDH12;
    union { 
        Uns16 value;
    } MCTRL12;
    union { 
        Uns32 value;
    } MDATA0113;
    union { 
        Uns32 value;
    } MDATA2313;
    union { 
        Uns32 value;
    } MDATA4513;
    union { 
        Uns32 value;
    } MDATA6713;
    union { 
        Uns16 value;
    } MIDL13;
    union { 
        Uns16 value;
    } MIDH13;
    union { 
        Uns16 value;
    } MCTRL13;
    union { 
        Uns32 value;
    } MDATA0114;
    union { 
        Uns32 value;
    } MDATA2314;
    union { 
        Uns32 value;
    } MDATA4514;
    union { 
        Uns32 value;
    } MDATA6714;
    union { 
        Uns16 value;
    } MIDL14;
    union { 
        Uns16 value;
    } MIDH14;
    union { 
        Uns16 value;
    } MCTRL14;
    union { 
        Uns32 value;
    } MDATA0115;
    union { 
        Uns32 value;
    } MDATA2315;
    union { 
        Uns32 value;
    } MDATA4515;
    union { 
        Uns32 value;
    } MDATA6715;
    union { 
        Uns16 value;
    } MIDL15;
    union { 
        Uns16 value;
    } MIDH15;
    union { 
        Uns16 value;
    } MCTRL15;
    union { 
        Uns32 value;
    } MDATA0116;
    union { 
        Uns32 value;
    } MDATA2316;
    union { 
        Uns32 value;
    } MDATA4516;
    union { 
        Uns32 value;
    } MDATA6716;
    union { 
        Uns16 value;
    } MIDL16;
    union { 
        Uns16 value;
    } MIDH16;
    union { 
        Uns16 value;
    } MCTRL16;
    union { 
        Uns32 value;
    } MDATA0117;
    union { 
        Uns32 value;
    } MDATA2317;
    union { 
        Uns32 value;
    } MDATA4517;
    union { 
        Uns32 value;
    } MDATA6717;
    union { 
        Uns16 value;
    } MIDL17;
    union { 
        Uns16 value;
    } MIDH17;
    union { 
        Uns16 value;
    } MCTRL17;
    union { 
        Uns32 value;
    } MDATA0118;
    union { 
        Uns32 value;
    } MDATA2318;
    union { 
        Uns32 value;
    } MDATA4518;
    union { 
        Uns32 value;
    } MDATA6718;
    union { 
        Uns16 value;
    } MIDL18;
    union { 
        Uns16 value;
    } MIDH18;
    union { 
        Uns16 value;
    } MCTRL18;
    union { 
        Uns32 value;
    } MDATA0119;
    union { 
        Uns32 value;
    } MDATA2319;
    union { 
        Uns32 value;
    } MDATA4519;
    union { 
        Uns32 value;
    } MDATA6719;
    union { 
        Uns16 value;
    } MIDL19;
    union { 
        Uns16 value;
    } MIDH19;
    union { 
        Uns16 value;
    } MCTRL19;
    union { 
        Uns32 value;
    } MDATA0120;
    union { 
        Uns32 value;
    } MDATA2320;
    union { 
        Uns32 value;
    } MDATA4520;
    union { 
        Uns32 value;
    } MDATA6720;
    union { 
        Uns16 value;
    } MIDL20;
    union { 
        Uns16 value;
    } MIDH20;
    union { 
        Uns16 value;
    } MCTRL20;
    union { 
        Uns32 value;
    } MDATA0121;
    union { 
        Uns32 value;
    } MDATA2321;
    union { 
        Uns32 value;
    } MDATA4521;
    union { 
        Uns32 value;
    } MDATA6721;
    union { 
        Uns16 value;
    } MIDL21;
    union { 
        Uns16 value;
    } MIDH21;
    union { 
        Uns16 value;
    } MCTRL21;
    union { 
        Uns32 value;
    } MDATA0122;
    union { 
        Uns32 value;
    } MDATA2322;
    union { 
        Uns32 value;
    } MDATA4522;
    union { 
        Uns32 value;
    } MDATA6722;
    union { 
        Uns16 value;
    } MIDL22;
    union { 
        Uns16 value;
    } MIDH22;
    union { 
        Uns16 value;
    } MCTRL22;
    union { 
        Uns32 value;
    } MDATA0123;
    union { 
        Uns32 value;
    } MDATA2323;
    union { 
        Uns32 value;
    } MDATA4523;
    union { 
        Uns32 value;
    } MDATA6723;
    union { 
        Uns16 value;
    } MIDL23;
    union { 
        Uns16 value;
    } MIDH23;
    union { 
        Uns16 value;
    } MCTRL23;
    union { 
        Uns32 value;
    } MDATA0124;
    union { 
        Uns32 value;
    } MDATA2324;
    union { 
        Uns32 value;
    } MDATA4524;
    union { 
        Uns32 value;
    } MDATA6724;
    union { 
        Uns16 value;
    } MIDL24;
    union { 
        Uns16 value;
    } MIDH24;
    union { 
        Uns16 value;
    } MCTRL24;
    union { 
        Uns32 value;
    } MDATA0125;
    union { 
        Uns32 value;
    } MDATA2325;
    union { 
        Uns32 value;
    } MDATA4525;
    union { 
        Uns32 value;
    } MDATA6725;
    union { 
        Uns16 value;
    } MIDL25;
    union { 
        Uns16 value;
    } MIDH25;
    union { 
        Uns16 value;
    } MCTRL25;
    union { 
        Uns32 value;
    } MDATA0126;
    union { 
        Uns32 value;
    } MDATA2326;
    union { 
        Uns32 value;
    } MDATA4526;
    union { 
        Uns32 value;
    } MDATA6726;
    union { 
        Uns16 value;
    } MIDL26;
    union { 
        Uns16 value;
    } MIDH26;
    union { 
        Uns16 value;
    } MCTRL26;
    union { 
        Uns32 value;
    } MDATA0127;
    union { 
        Uns32 value;
    } MDATA2327;
    union { 
        Uns32 value;
    } MDATA4527;
    union { 
        Uns32 value;
    } MDATA6727;
    union { 
        Uns16 value;
    } MIDL27;
    union { 
        Uns16 value;
    } MIDH27;
    union { 
        Uns16 value;
    } MCTRL27;
    union { 
        Uns32 value;
    } MDATA0128;
    union { 
        Uns32 value;
    } MDATA2328;
    union { 
        Uns32 value;
    } MDATA4528;
    union { 
        Uns32 value;
    } MDATA6728;
    union { 
        Uns16 value;
    } MIDL28;
    union { 
        Uns16 value;
    } MIDH28;
    union { 
        Uns16 value;
    } MCTRL28;
    union { 
        Uns32 value;
    } MDATA0129;
    union { 
        Uns32 value;
    } MDATA2329;
    union { 
        Uns32 value;
    } MDATA4529;
    union { 
        Uns32 value;
    } MDATA6729;
    union { 
        Uns16 value;
    } MIDL29;
    union { 
        Uns16 value;
    } MIDH29;
    union { 
        Uns16 value;
    } MCTRL29;
    union { 
        Uns32 value;
    } MDATA0130;
    union { 
        Uns32 value;
    } MDATA2330;
    union { 
        Uns32 value;
    } MDATA4530;
    union { 
        Uns32 value;
    } MDATA6730;
    union { 
        Uns16 value;
    } MIDL30;
    union { 
        Uns16 value;
    } MIDH30;
    union { 
        Uns16 value;
    } MCTRL30;
    union { 
        Uns32 value;
    } MDATA0131;
    union { 
        Uns32 value;
    } MDATA2331;
    union { 
        Uns32 value;
    } MDATA4531;
    union { 
        Uns32 value;
    } MDATA6731;
    union { 
        Uns16 value;
    } MIDL31;
    union { 
        Uns16 value;
    } MIDH31;
    union { 
        Uns16 value;
    } MCTRL31;
} busPortMDH_MDH_dataT, *busPortMDH_MDH_dataTP;

typedef struct busPortMDW_MDW_dataS { 
    union { 
        Uns32 value;
    } MDATA012300;
    union { 
        Uns32 value;
    } MDATA456700;
    union { 
        Uns32 value;
    } MID00;
    union { 
        Uns32 value;
    } MDATA012301;
    union { 
        Uns32 value;
    } MDATA456701;
    union { 
        Uns32 value;
    } MID01;
    union { 
        Uns32 value;
    } MDATA012302;
    union { 
        Uns32 value;
    } MDATA456702;
    union { 
        Uns32 value;
    } MID02;
    union { 
        Uns32 value;
    } MDATA012303;
    union { 
        Uns32 value;
    } MDATA456703;
    union { 
        Uns32 value;
    } MID03;
    union { 
        Uns32 value;
    } MDATA012304;
    union { 
        Uns32 value;
    } MDATA456704;
    union { 
        Uns32 value;
    } MID04;
    union { 
        Uns32 value;
    } MDATA012305;
    union { 
        Uns32 value;
    } MDATA456705;
    union { 
        Uns32 value;
    } MID05;
    union { 
        Uns32 value;
    } MDATA012306;
    union { 
        Uns32 value;
    } MDATA456706;
    union { 
        Uns32 value;
    } MID06;
    union { 
        Uns32 value;
    } MDATA012307;
    union { 
        Uns32 value;
    } MDATA456707;
    union { 
        Uns32 value;
    } MID07;
    union { 
        Uns32 value;
    } MDATA012308;
    union { 
        Uns32 value;
    } MDATA456708;
    union { 
        Uns32 value;
    } MID08;
    union { 
        Uns32 value;
    } MDATA012309;
    union { 
        Uns32 value;
    } MDATA456709;
    union { 
        Uns32 value;
    } MID09;
    union { 
        Uns32 value;
    } MDATA012310;
    union { 
        Uns32 value;
    } MDATA456710;
    union { 
        Uns32 value;
    } MID10;
    union { 
        Uns32 value;
    } MDATA012311;
    union { 
        Uns32 value;
    } MDATA456711;
    union { 
        Uns32 value;
    } MID11;
    union { 
        Uns32 value;
    } MDATA012312;
    union { 
        Uns32 value;
    } MDATA456712;
    union { 
        Uns32 value;
    } MID12;
    union { 
        Uns32 value;
    } MDATA012313;
    union { 
        Uns32 value;
    } MDATA456713;
    union { 
        Uns32 value;
    } MID13;
    union { 
        Uns32 value;
    } MDATA012314;
    union { 
        Uns32 value;
    } MDATA456714;
    union { 
        Uns32 value;
    } MID14;
    union { 
        Uns32 value;
    } MDATA012315;
    union { 
        Uns32 value;
    } MDATA456715;
    union { 
        Uns32 value;
    } MID15;
    union { 
        Uns32 value;
    } MDATA012316;
    union { 
        Uns32 value;
    } MDATA456716;
    union { 
        Uns32 value;
    } MID16;
    union { 
        Uns32 value;
    } MDATA012317;
    union { 
        Uns32 value;
    } MDATA456717;
    union { 
        Uns32 value;
    } MID17;
    union { 
        Uns32 value;
    } MDATA012318;
    union { 
        Uns32 value;
    } MDATA456718;
    union { 
        Uns32 value;
    } MID18;
    union { 
        Uns32 value;
    } MDATA012319;
    union { 
        Uns32 value;
    } MDATA456719;
    union { 
        Uns32 value;
    } MID19;
    union { 
        Uns32 value;
    } MDATA012320;
    union { 
        Uns32 value;
    } MDATA456720;
    union { 
        Uns32 value;
    } MID20;
    union { 
        Uns32 value;
    } MDATA012321;
    union { 
        Uns32 value;
    } MDATA456721;
    union { 
        Uns32 value;
    } MID21;
    union { 
        Uns32 value;
    } MDATA012322;
    union { 
        Uns32 value;
    } MDATA456722;
    union { 
        Uns32 value;
    } MID22;
    union { 
        Uns32 value;
    } MDATA012323;
    union { 
        Uns32 value;
    } MDATA456723;
    union { 
        Uns32 value;
    } MID23;
    union { 
        Uns32 value;
    } MDATA012324;
    union { 
        Uns32 value;
    } MDATA456724;
    union { 
        Uns32 value;
    } MID24;
    union { 
        Uns32 value;
    } MDATA012325;
    union { 
        Uns32 value;
    } MDATA456725;
    union { 
        Uns32 value;
    } MID25;
    union { 
        Uns32 value;
    } MDATA012326;
    union { 
        Uns32 value;
    } MDATA456726;
    union { 
        Uns32 value;
    } MID26;
    union { 
        Uns32 value;
    } MDATA012327;
    union { 
        Uns32 value;
    } MDATA456727;
    union { 
        Uns32 value;
    } MID27;
    union { 
        Uns32 value;
    } MDATA012328;
    union { 
        Uns32 value;
    } MDATA456728;
    union { 
        Uns32 value;
    } MID28;
    union { 
        Uns32 value;
    } MDATA012329;
    union { 
        Uns32 value;
    } MDATA456729;
    union { 
        Uns32 value;
    } MID29;
    union { 
        Uns32 value;
    } MDATA012330;
    union { 
        Uns32 value;
    } MDATA456730;
    union { 
        Uns32 value;
    } MID30;
    union { 
        Uns32 value;
    } MDATA012331;
    union { 
        Uns32 value;
    } MDATA456731;
    union { 
        Uns32 value;
    } MID31;
} busPortMDW_MDW_dataT, *busPortMDW_MDW_dataTP;

typedef struct busPortECC_ECC_dataS { 
    union { 
        Uns16 value;
    } E6A0CTL;
} busPortECC_ECC_dataT, *busPortECC_ECC_dataTP;

typedef struct busPortGR_GR_dataS { 
    union { 
        Uns16 value;
    } GMCTRL;
    union { 
        Uns16 value;
    } GMABT;
    union { 
        Uns16 value;
    } GMCONF;
} busPortGR_GR_dataT, *busPortGR_GR_dataTP;

typedef struct busPortCR_CR_dataS { 
    union { 
        Uns16 value;
    } MASK1L;
    union { 
        Uns16 value;
    } MASK1H;
    union { 
        Uns16 value;
    } MASK2L;
    union { 
        Uns16 value;
    } MASK2H;
    union { 
        Uns16 value;
    } MASK3L;
    union { 
        Uns16 value;
    } MASK3H;
    union { 
        Uns16 value;
    } MASK4L;
    union { 
        Uns16 value;
    } MASK4H;
    union { 
        Uns16 value;
    } CTRL;
    union { 
        Uns16 value;
    } ERC;
    union { 
        Uns16 value;
    } IE;
    union { 
        Uns16 value;
    } INTS;
    union { 
        Uns16 value;
    } BTR;
    union { 
        Uns16 value;
    } RGPT;
    union { 
        Uns16 value;
    } TGPT;
    union { 
        Uns16 value;
    } TS;
} busPortCR_CR_dataT, *busPortCR_CR_dataTP;

typedef struct busPortCRW_CRW_dataS { 
    union { 
        Uns16 value;
    } MASK1;
    union { 
        Uns16 value;
    } MASK2;
    union { 
        Uns16 value;
    } MASK3;
    union { 
        Uns16 value;
    } MASK4;
} busPortCRW_CRW_dataT, *busPortCRW_CRW_dataTP;

/////////////////////////////// Port Declarations //////////////////////////////

extern busPortMR_MR_dataT busPortMR_MR_data;

extern busPortMDB_MDB_dataT busPortMDB_MDB_data;

extern busPortMDH_MDH_dataT busPortMDH_MDH_data;

extern busPortMDW_MDW_dataT busPortMDW_MDW_data;

extern busPortECC_ECC_dataT busPortECC_ECC_data;

extern busPortGR_GR_dataT busPortGR_GR_data;

extern busPortCR_CR_dataT busPortCR_CR_data;

extern busPortCRW_CRW_dataT busPortCRW_CRW_data;

#ifdef _PSE_
///////////////////////////////// Port handles /////////////////////////////////

typedef struct handlesS {
    void                 *busPortMR;
    void                 *busPortMDB;
    void                 *busPortMDH;
    void                 *busPortMDW;
    void                 *busPortECC;
    void                 *busPortGR;
    void                 *busPortCR;
    void                 *busPortCRW;
    ppmNetHandle          INTTRX;
    ppmNetHandle          INTREC;
    ppmNetHandle          INTERR;
    ppmNetHandle          INTWUP;
    ppmPacketnetHandle    CAN;
} handlesT, *handlesTP;

extern handlesT handles;

////////////////////////////// Callback prototypes /////////////////////////////

PPM_PACKETNET_CB(canrx);
PPM_REG_READ_CB(readBRP);
PPM_REG_READ_CB(readBTR);
PPM_REG_READ_CB(readCTRL);
PPM_REG_READ_CB(readE6A0CTL);
PPM_REG_READ_CB(readERC);
PPM_REG_READ_CB(readGMABT);
PPM_REG_READ_CB(readGMABTD);
PPM_REG_READ_CB(readGMCONF);
PPM_REG_READ_CB(readGMCS);
PPM_REG_READ_CB(readGMCTRL);
PPM_REG_READ_CB(readIE);
PPM_REG_READ_CB(readINFO);
PPM_REG_READ_CB(readINTS);
PPM_REG_READ_CB(readLEC);
PPM_REG_READ_CB(readLIPT);
PPM_REG_READ_CB(readLOPT);
PPM_REG_READ_CB(readMASK);
PPM_REG_READ_CB(readMASKH);
PPM_REG_READ_CB(readMASKL);
PPM_REG_READ_CB(readMCONF);
PPM_REG_READ_CB(readMCTRL);
PPM_REG_READ_CB(readMDATA);
PPM_REG_READ_CB(readMDLC);
PPM_REG_READ_CB(readMID);
PPM_REG_READ_CB(readMIDH);
PPM_REG_READ_CB(readMIDL);
PPM_REG_READ_CB(readRGPT);
PPM_REG_READ_CB(readTGPT);
PPM_REG_READ_CB(readTS);
PPM_REG_VIEW_CB(viewBRP);
PPM_REG_VIEW_CB(viewBTR);
PPM_REG_VIEW_CB(viewCTRL);
PPM_REG_VIEW_CB(viewE6A0CTL);
PPM_REG_VIEW_CB(viewERC);
PPM_REG_VIEW_CB(viewGMABT);
PPM_REG_VIEW_CB(viewGMABTD);
PPM_REG_VIEW_CB(viewGMCONF);
PPM_REG_VIEW_CB(viewGMCS);
PPM_REG_VIEW_CB(viewGMCTRL);
PPM_REG_VIEW_CB(viewIE);
PPM_REG_VIEW_CB(viewINFO);
PPM_REG_VIEW_CB(viewINTS);
PPM_REG_VIEW_CB(viewLEC);
PPM_REG_VIEW_CB(viewLIPT);
PPM_REG_VIEW_CB(viewLOPT);
PPM_REG_VIEW_CB(viewMASK);
PPM_REG_VIEW_CB(viewMASKH);
PPM_REG_VIEW_CB(viewMASKL);
PPM_REG_VIEW_CB(viewMCONF);
PPM_REG_VIEW_CB(viewMCTRL);
PPM_REG_VIEW_CB(viewMDLC);
PPM_REG_VIEW_CB(viewMID);
PPM_REG_VIEW_CB(viewMIDH);
PPM_REG_VIEW_CB(viewMIDL);
PPM_REG_VIEW_CB(viewRGPT);
PPM_REG_VIEW_CB(viewTGPT);
PPM_REG_VIEW_CB(viewTS);
PPM_REG_WRITE_CB(writeBRP);
PPM_REG_WRITE_CB(writeBTR);
PPM_REG_WRITE_CB(writeCTRL);
PPM_REG_WRITE_CB(writeE6A0CTL);
PPM_REG_WRITE_CB(writeERC);
PPM_REG_WRITE_CB(writeGMABT);
PPM_REG_WRITE_CB(writeGMABTD);
PPM_REG_WRITE_CB(writeGMCS);
PPM_REG_WRITE_CB(writeGMCTRL);
PPM_REG_WRITE_CB(writeIE);
PPM_REG_WRITE_CB(writeINFO);
PPM_REG_WRITE_CB(writeINTS);
PPM_REG_WRITE_CB(writeLEC);
PPM_REG_WRITE_CB(writeMASK);
PPM_REG_WRITE_CB(writeMASKH);
PPM_REG_WRITE_CB(writeMASKL);
PPM_REG_WRITE_CB(writeMCONF);
PPM_REG_WRITE_CB(writeMCTRL);
PPM_REG_WRITE_CB(writeMDATA);
PPM_REG_WRITE_CB(writeMDLC);
PPM_REG_WRITE_CB(writeMID);
PPM_REG_WRITE_CB(writeMIDH);
PPM_REG_WRITE_CB(writeMIDL);
PPM_REG_WRITE_CB(writeRGPT);
PPM_REG_WRITE_CB(writeTGPT);
PPM_REG_WRITE_CB(writeTS);
PPM_CONSTRUCTOR_CB(periphConstructor);
PPM_CONSTRUCTOR_CB(constructor);
PPM_SAVE_STATE_FN(peripheralSaveState);
PPM_RESTORE_STATE_FN(peripheralRestoreState);

#endif

#endif
