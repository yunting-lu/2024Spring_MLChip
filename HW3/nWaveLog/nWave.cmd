wvRestoreSignal -win $_nWave1 \
           "/RAID2/COURSE/mlchip/mlchip098/HW3_upload_v1/signal.rc" \
           -overWriteAutoAlias on -appendSignals on
wvConvertFile -win $_nWave1 -o "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb" \
           "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd"
wvSelectSignal -win $_nWave1 {( "core0" 6 )} 
wvSelectSignal -win $_nWave1 {( "core0" 6 7 8 )} 
wvSetPosition -win $_nWave1 {("core0" 6)}
wvSetPosition -win $_nWave1 {("core0" 2)}
wvMoveSelected -win $_nWave1
wvSetPosition -win $_nWave1 {("core0" 2)}
wvSetPosition -win $_nWave1 {("core0" 5)}
wvSelectSignal -win $_nWave1 {( "router0" 3 )} 
wvSelectSignal -win $_nWave1 {( "router0" 5 6 )} 
wvSetPosition -win $_nWave1 {("router0" 5)}
wvSelectSignal -win $_nWave1 {( "router0" 5 )} 
wvMoveSelected -win $_nWave1
wvSetPosition -win $_nWave1 {("router0" 5)}
wvResizeWindow -win $_nWave1 0 23 1536 793
wvSetCursor -win $_nWave1 109395.332455 -snap {("core0" 3)}
wvResizeWindow -win $_nWave1 0 23 1536 793
wvZoom -win $_nWave1 89884.170552 144896.890321
wvZoomOut -win $_nWave1
wvZoomOut -win $_nWave1
wvSelectSignal -win $_nWave1 {( "core0" 5 )} 
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollUp -win $_nWave1 1
wvScrollUp -win $_nWave1 1
wvScrollUp -win $_nWave1 1
wvScrollUp -win $_nWave1 1
wvScrollUp -win $_nWave1 1
wvScrollDown -win $_nWave1 0
wvScrollDown -win $_nWave1 0
wvScrollDown -win $_nWave1 0
wvScrollDown -win $_nWave1 0
wvScrollDown -win $_nWave1 0
wvScrollDown -win $_nWave1 0
wvSetCursor -win $_nWave1 220681.503484 -snap {("core0" 3)}
wvSetCursor -win $_nWave1 222080.873145 -snap {("router0" 3)}
wvSelectSignal -win $_nWave1 {( "router0" 5 )} 
wvResizeWindow -win $_nWave1 0 23 1536 793
wvResizeWindow -win $_nWave1 0 23 1536 793
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvSelectSignal -win $_nWave1 {( "router0" 29 )} 
wvSetPosition -win $_nWave1 {("router0" 29)}
wvSetPosition -win $_nWave1 {("router0" 32)}
wvSetPosition -win $_nWave1 {("router0" 33)}
wvMoveSelected -win $_nWave1
wvSetPosition -win $_nWave1 {("router0" 33)}
wvSelectSignal -win $_nWave1 {( "router0" 39 )} 
wvSetPosition -win $_nWave1 {("router0" 39)}
wvSetPosition -win $_nWave1 {("router0" 36)}
wvSetPosition -win $_nWave1 {("router0" 34)}
wvMoveSelected -win $_nWave1
wvSetPosition -win $_nWave1 {("router0" 34)}
wvSetPosition -win $_nWave1 {("router0" 35)}
wvSelectSignal -win $_nWave1 {( "router0" 38 )} 
wvScrollUp -win $_nWave1 16
wvSelectSignal -win $_nWave1 {( "core0" 2 )} 
wvSetPosition -win $_nWave1 {("core0" 2)}
wvSetPosition -win $_nWave1 {("core0" 7)}
wvSetPosition -win $_nWave1 {("core0" 8)}
wvSetPosition -win $_nWave1 {("core0" 4)}
wvSetPosition -win $_nWave1 {("core0" 1)}
wvSetPosition -win $_nWave1 {("core0" 0)}
wvMoveSelected -win $_nWave1
wvSetPosition -win $_nWave1 {("core0" 0)}
wvSetPosition -win $_nWave1 {("core0" 1)}
wvSelectSignal -win $_nWave1 {( "core0" 1 )} 
wvSelectSignal -win $_nWave1 {( "router0" 8 )} 
wvSetPosition -win $_nWave1 {("router0" 8)}
wvSetPosition -win $_nWave1 {("router0" 7)}
wvSetPosition -win $_nWave1 {("router0" 8)}
wvMoveSelected -win $_nWave1
wvSetPosition -win $_nWave1 {("router0" 8)}
wvSelectSignal -win $_nWave1 {( "router0" 8 )} 
wvSelectSignal -win $_nWave1 {( "core0" 2 )} 
wvSetPosition -win $_nWave1 {("core0" 2)}
wvSetPosition -win $_nWave1 {("core0" 4)}
wvSetPosition -win $_nWave1 {("core0" 5)}
wvMoveSelected -win $_nWave1
wvSetPosition -win $_nWave1 {("core0" 5)}
wvSelectSignal -win $_nWave1 {( "router0" 6 )} 
wvSetPosition -win $_nWave1 {("router0" 6)}
wvSetPosition -win $_nWave1 {("router0" 5)}
wvSetPosition -win $_nWave1 {("router0" 6)}
wvMoveSelected -win $_nWave1
wvSetPosition -win $_nWave1 {("router0" 6)}
wvSelectSignal -win $_nWave1 {( "router0" 6 )} 
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 1
wvScrollDown -win $_nWave1 11
wvSelectSignal -win $_nWave1 {( "router0" 29 )} 
wvSetPosition -win $_nWave1 {("router0" 29)}
wvSetPosition -win $_nWave1 {("router0" 36)}
wvSetPosition -win $_nWave1 {("router0" 39)}
wvMoveSelected -win $_nWave1
wvSetPosition -win $_nWave1 {("router0" 39)}
wvSelectSignal -win $_nWave1 {( "router0" 35 )} 
wvSetPosition -win $_nWave1 {("router0" 35)}
wvSetPosition -win $_nWave1 {("router0" 39)}
wvMoveSelected -win $_nWave1
wvSetPosition -win $_nWave1 {("router0" 39)}
wvZoomOut -win $_nWave1
wvSelectSignal -win $_nWave1 {( "router0" 34 )} 
wvSelectSignal -win $_nWave1 {( "router0" 32 )} 
wvScrollUp -win $_nWave1 19
wvSelectSignal -win $_nWave1 {( "core0" 5 )} 
wvAddSignal -win $_nWave1 "/SystemC/clk"
wvSetPosition -win $_nWave1 {("router0" 39)}
wvSetPosition -win $_nWave1 {("router0" 40)}
wvSetPosition -win $_nWave1 {("router0" 36)}
wvScrollDown -win $_nWave1 1
wvSetPosition -win $_nWave1 {("router0" 31)}
wvMoveSelected -win $_nWave1
wvSetPosition -win $_nWave1 {("router0" 31)}
wvSetPosition -win $_nWave1 {("router0" 32)}
wvSelectSignal -win $_nWave1 {( "router0" 40 )} 
wvZoomIn -win $_nWave1
wvSelectSignal -win $_nWave1 {( "router0" 41 )} 
wvSetPosition -win $_nWave1 {("router0" 41)}
wvMoveSelected -win $_nWave1
wvSetPosition -win $_nWave1 {("router0" 41)}
wvSelectSignal -win $_nWave1 {( "router0" 41 )} 
wvExit
