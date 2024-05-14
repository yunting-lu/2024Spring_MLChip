wvConvertFile -win $_nWave1 -o "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb" \
           "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd"
wvSetPosition -win $_nWave1 {("G1" 0)}
wvOpenFile -win $_nWave1 {/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb}
wvGetSignalOpen -win $_nWave1
wvGetSignalSetScope -win $_nWave1 "/SystemC"
wvGetSignalSetScope -win $_nWave1 "/SystemC/core_0"
wvGetSignalSetScope -win $_nWave1 "/SystemC/core_1"
wvGetSignalSetScope -win $_nWave1 "/SystemC/core_2"
wvGetSignalSetScope -win $_nWave1 "/SystemC/core_3"
wvGetSignalSetScope -win $_nWave1 "/SystemC/core_5"
wvGetSignalSetScope -win $_nWave1 "/SystemC/core_8"
wvGetSignalSetScope -win $_nWave1 "/SystemC/core_5"
wvGetSignalSetScope -win $_nWave1 "/SystemC/core_3"
wvGetSignalSetScope -win $_nWave1 "/SystemC/router_6"
wvGetSignalSetScope -win $_nWave1 "/SystemC/router_4"
wvGetSignalSetScope -win $_nWave1 "/SystemC/core_14"
wvGetSignalSetScope -win $_nWave1 "/SystemC/router_8"
wvGetSignalClose -win $_nWave1
wvResizeWindow -win $_nWave1 0 23 1536 793
wvGetSignalOpen -win $_nWave1
wvGetSignalSetScope -win $_nWave1 "/SystemC"
wvGetSignalSetScope -win $_nWave1 "/SystemC/router_8"
wvGetSignalSetScope -win $_nWave1 "/SystemC/core_0"
wvSetPosition -win $_nWave1 {("G1" 3)}
wvSetPosition -win $_nWave1 {("G1" 3)}
wvAddSignal -win $_nWave1 -clear
wvAddSignal -win $_nWave1 -group {"G1" \
{/SystemC/core_0/ack_tx} \
{/SystemC/core_0/flit_tx\[33:0\]} \
{/SystemC/core_0/req_tx} \
}
wvAddSignal -win $_nWave1 -group {"G2" \
}
wvSelectSignal -win $_nWave1 {( "G1" 1 2 3 )} 
wvSetPosition -win $_nWave1 {("G1" 3)}
wvSetPosition -win $_nWave1 {("G1" 6)}
wvSetPosition -win $_nWave1 {("G1" 6)}
wvAddSignal -win $_nWave1 -clear
wvAddSignal -win $_nWave1 -group {"G1" \
{/SystemC/core_0/ack_tx} \
{/SystemC/core_0/flit_tx\[33:0\]} \
{/SystemC/core_0/req_tx} \
{/SystemC/core_0/ack_rx} \
{/SystemC/core_0/flit_rx\[33:0\]} \
{/SystemC/core_0/req_rx} \
}
wvAddSignal -win $_nWave1 -group {"G2" \
}
wvSelectSignal -win $_nWave1 {( "G1" 4 5 6 )} 
wvSetPosition -win $_nWave1 {("G1" 6)}
wvSetPosition -win $_nWave1 {("G1" 6)}
wvSetPosition -win $_nWave1 {("G1" 6)}
wvAddSignal -win $_nWave1 -clear
wvAddSignal -win $_nWave1 -group {"G1" \
{/SystemC/core_0/ack_tx} \
{/SystemC/core_0/flit_tx\[33:0\]} \
{/SystemC/core_0/req_tx} \
{/SystemC/core_0/ack_rx} \
{/SystemC/core_0/flit_rx\[33:0\]} \
{/SystemC/core_0/req_rx} \
}
wvAddSignal -win $_nWave1 -group {"G2" \
}
wvSelectSignal -win $_nWave1 {( "G1" 4 5 6 )} 
wvSetPosition -win $_nWave1 {("G1" 6)}
wvGetSignalClose -win $_nWave1
wvZoomOut -win $_nWave1
wvZoomOut -win $_nWave1
wvResizeWindow -win $_nWave1 0 23 1536 793
wvSelectSignal -win $_nWave1 {( "G1" 3 )} 
wvZoomOut -win $_nWave1
wvZoomOut -win $_nWave1
wvZoomOut -win $_nWave1
wvDisplayGridCount -win $_nWave1 -off
wvGetSignalClose -win $_nWave1
wvConvertFile -win $_nWave1 -o "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb" \
           "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd"
wvReloadFile -win $_nWave1
wvZoomOut -win $_nWave1
wvDisplayGridCount -win $_nWave1 -off
wvGetSignalClose -win $_nWave1
wvConvertFile -win $_nWave1 -o "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb" \
           "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd"
wvReloadFile -win $_nWave1
wvZoomOut -win $_nWave1
wvZoomOut -win $_nWave1
wvZoomOut -win $_nWave1
wvDisplayGridCount -win $_nWave1 -off
wvGetSignalClose -win $_nWave1
wvConvertFile -win $_nWave1 -o "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb" \
           "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd"
wvReloadFile -win $_nWave1
wvDisplayGridCount -win $_nWave1 -off
wvGetSignalClose -win $_nWave1
wvConvertFile -win $_nWave1 -o "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb" \
           "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd"
wvReloadFile -win $_nWave1
wvZoomOut -win $_nWave1
wvDisplayGridCount -win $_nWave1 -off
wvGetSignalClose -win $_nWave1
wvConvertFile -win $_nWave1 -o "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb" \
           "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd"
wvReloadFile -win $_nWave1
wvZoomOut -win $_nWave1
wvZoomOut -win $_nWave1
wvZoomOut -win $_nWave1
wvZoomIn -win $_nWave1
wvZoomIn -win $_nWave1
wvZoomOut -win $_nWave1
wvZoomOut -win $_nWave1
wvZoomOut -win $_nWave1
wvZoomOut -win $_nWave1
wvDisplayGridCount -win $_nWave1 -off
wvGetSignalClose -win $_nWave1
wvConvertFile -win $_nWave1 -o "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb" \
           "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd"
wvReloadFile -win $_nWave1
wvDisplayGridCount -win $_nWave1 -off
wvGetSignalClose -win $_nWave1
wvConvertFile -win $_nWave1 -o "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb" \
           "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd"
wvReloadFile -win $_nWave1
wvZoomIn -win $_nWave1
wvDisplayGridCount -win $_nWave1 -off
wvGetSignalClose -win $_nWave1
wvConvertFile -win $_nWave1 -o "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb" \
           "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd"
wvReloadFile -win $_nWave1
wvZoomOut -win $_nWave1
wvZoomIn -win $_nWave1
wvDisplayGridCount -win $_nWave1 -off
wvGetSignalClose -win $_nWave1
wvConvertFile -win $_nWave1 -o "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb" \
           "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd"
wvReloadFile -win $_nWave1
wvZoomOut -win $_nWave1
wvDisplayGridCount -win $_nWave1 -off
wvGetSignalClose -win $_nWave1
wvConvertFile -win $_nWave1 -o "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd.fsdb" \
           "/RAID2/COURSE/mlchip/mlchip098/hw3/wave.vcd"
wvReloadFile -win $_nWave1
