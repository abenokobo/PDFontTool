SET PDFT="pdft/bin/x64/pdft.exe"
SET OUTPUT_DIR="Source/fonts/"

rd /S /Q %OUTPUT_DIR%
mkdir %OUTPUT_DIR%

exit

rem Create a font in %OUTPUT_DIR% and it will appear in PDFontTool.pdx.
rem Below are the fonts used for the test.

rem %OUTPUT_DIR% にフォントを作成すると、PDFontTool.pdx で表示されます。
rem 以下、試験に利用したフォントです。

%PDFT% "test-data/M_PLUS_Rounded_1c/MPLUSRounded1c-Thin.ttf" %OUTPUT_DIR% "MPLUSRounded1c-Thin16" 16
%PDFT% "test-data/M_PLUS_Rounded_1c/MPLUSRounded1c-Thin.ttf" %OUTPUT_DIR% "MPLUSRounded1c-Thin24" 24
%PDFT% "test-data/M_PLUS_Rounded_1c/MPLUSRounded1c-Thin.ttf" %OUTPUT_DIR% "MPLUSRounded1c-Thin32" 32
%PDFT% "test-data/Noto_Sans_JP/NotoSansJP-Thin.otf" %OUTPUT_DIR% "NotoSansJP-Thin16" 16
%PDFT% "test-data/Noto_Sans_JP/NotoSansJP-Thin.otf" %OUTPUT_DIR% "NotoSansJP-Thin24" 24
%PDFT% "test-data/Noto_Sans_JP/NotoSansJP-Thin.otf" %OUTPUT_DIR% "NotoSansJP-Thin32" 32
%PDFT% "test-data/Anton/Anton-Regular.ttf" %OUTPUT_DIR% "Anton-Regular24" 24
%PDFT% "test-data/Bebas_Neue/BebasNeue-Regular.ttf" %OUTPUT_DIR% "BebasNeue-Regular24" 24
%PDFT% "test-data/Dela_Gothic_One/DelaGothicOne-Regular.ttf" %OUTPUT_DIR% "DelaGothicOne-Regular32" 32
%PDFT% "test-data/Kosugi/Kosugi-Regular.ttf" %OUTPUT_DIR% "Kosugi-Regular24" 24
%PDFT% "test-data/Kosugi_Maru/KosugiMaru-Regular.ttf" %OUTPUT_DIR% "KosugiMaru-Regular24" 24
%PDFT% "test-data/Lato/Lato-BoldItalic.ttf" %OUTPUT_DIR% "Lato-BoldItalic24" 24
%PDFT% "test-data/Lobster/Lobster-Regular.ttf" %OUTPUT_DIR% "Lobster-Regular24" 24
%PDFT% "test-data/M_PLUS_1p/MPLUS1p-Regular.ttf" %OUTPUT_DIR% "MPLUS1p-Regular24" 24
%PDFT% "test-data/minion-pro-cufonfonts/MinionPro-Regular.otf" %OUTPUT_DIR% "MinionPro-Regular24" 24
%PDFT% "test-data/Montserrat/Montserrat-VariableFont_wght.ttf" %OUTPUT_DIR% "Montserrat-VariableFont_wght24" 24
%PDFT% "test-data/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf" %OUTPUT_DIR% "OpenSans-VariableFont_wdth,wght24" 24
%PDFT% "test-data/Poppins/Poppins-Regular.ttf" %OUTPUT_DIR% "Poppins-Regular24" 24
%PDFT% "test-data/Reggae_One/ReggaeOne-Regular.ttf" %OUTPUT_DIR% "ReggaeOne-Regular24" 24
%PDFT% "test-data/Roboto/Roboto-Regular.ttf" %OUTPUT_DIR% "Roboto-Regular24" 24
%PDFT% "test-data/RocknRoll_One/RocknRollOne-Regular.ttf" %OUTPUT_DIR% "RocknRollOne-Regular24" 24
%PDFT% "test-data/Sawarabi_Gothic/SawarabiGothic-Regular.ttf" %OUTPUT_DIR% "SawarabiGothic-Regular24" 24
%PDFT% "test-data/Yanone_Kaffeesatz/YanoneKaffeesatz-VariableFont_wght.ttf" %OUTPUT_DIR% "YanoneKaffeesatz-VariableFont_wght24" 24
%PDFT% "test-data/Yusei_Magic/YuseiMagic-Regular.ttf" %OUTPUT_DIR% "YuseiMagic-Regular24" 24
%PDFT% "test-data/x12y16pxMaruMonica_2021-06-17/x12y16pxMaruMonica.ttf" %OUTPUT_DIR% "MaruMonica" 16
%PDFT% -R -ma 1 -mo 2 "test-data/jfdotfont-20150527/JF-Dot-ShinonomeMin14.ttf" %OUTPUT_DIR% "ShinonomeMin14" 14
%PDFT% -R -ma 1 -mo 2 "test-data/jfdotfont-20150527/JF-Dot-ShinonomeMin16.ttf" %OUTPUT_DIR% "ShinonomeMin16" 16
%PDFT% -R -ma 1 -mo 2 "test-data/jfdotfont-20150527/JF-Dot-ShinonomeMin16B.ttf" %OUTPUT_DIR% "ShinonomeMin16B" 16
%PDFT% -R -ma 1 -mo 2 "test-data/khdotfont-20150527/KH-Dot-Hibiya-24.ttf" %OUTPUT_DIR% "Hibiya-24" 24
%PDFT% -R -ma 1 -mo 2 "test-data/khdotfont-20150527/KH-Dot-Kagurazaka-16.ttf" %OUTPUT_DIR% "Kagurazaka-16" 16

