SET PDFT="pdft/bin/x64/pdft.exe"
SET OUTPUT_DIR="Source/fonts/"

rd /S /Q %OUTPUT_DIR%
mkdir %OUTPUT_DIR%

%PDFT% "test-data/M_PLUS_Rounded_1c/MPLUSRounded1c-Thin.ttf" %OUTPUT_DIR% "MPLUSRounded1c-Thin16" 16
%PDFT% "test-data/M_PLUS_Rounded_1c/MPLUSRounded1c-Thin.ttf" %OUTPUT_DIR% "MPLUSRounded1c-Thin24" 24
%PDFT% "test-data/M_PLUS_Rounded_1c/MPLUSRounded1c-Thin.ttf" %OUTPUT_DIR% "MPLUSRounded1c-Thin32" 32

%PDFT% "test-data/Noto_Sans_JP/NotoSansJP-Thin.otf" %OUTPUT_DIR% "NotoSansJP-Thin16" 16
%PDFT% "test-data/Noto_Sans_JP/NotoSansJP-Thin.otf" %OUTPUT_DIR% "NotoSansJP-Thin24" 24
%PDFT% "test-data/Noto_Sans_JP/NotoSansJP-Thin.otf" %OUTPUT_DIR% "NotoSansJP-Thin32" 32

%PDFT% "test-data/x12y16pxMaruMonica_2021-06-17/x12y16pxMaruMonica.ttf" %OUTPUT_DIR% "MaruMonica" 16

%PDFT% -R -ma 1 -mo 2 "test-data/jfdotfont-20150527/JF-Dot-ShinonomeMin14.ttf" %OUTPUT_DIR% "ShinonomeMin14" 14
%PDFT% -R -ma 1 -mo 2 "test-data/jfdotfont-20150527/JF-Dot-ShinonomeMin16.ttf" %OUTPUT_DIR% "ShinonomeMin16" 16
%PDFT% -R -ma 1 -mo 2 "test-data/jfdotfont-20150527/JF-Dot-ShinonomeMin16B.ttf" %OUTPUT_DIR% "ShinonomeMin16B" 16

%PDFT% -R -ma 1 -mo 2 "test-data/khdotfont-20150527/KH-Dot-Hibiya-24.ttf" %OUTPUT_DIR% "Hibiya-24" 24
%PDFT% -R -ma 1 -mo 2 "test-data/khdotfont-20150527/KH-Dot-Kagurazaka-16.ttf" %OUTPUT_DIR% "Kagurazaka-16" 16
