<CsoundSynthesizer>
<CsOptions>
</CsOptions>
<CsInstruments>


gituio TUIOinit 3333

instr 1  ; Receive TUIO

ktrigadd, ksymbol, kxpos, kypos, kangle, ksession addObject gituio

printf "add symbol= %i kxpos=%f kypos=%f kangle=%f ksession=%f\n", ktrigadd, ksymbol, kxpos, kypos, kangle, ksession

ktrigupdt, ksymbol, kxpos, kypos, kangle, kxspeed, kyspeed, krspeed, kmaccel, kraccel, ksession updateObject gituio
printf "update symbol= %i kxpos=%f kypos=%f kangle=%f kxspeed=%f kyspeed=%f krspeed=%f kmaccel= %f kraccel=%f ksession=%f\n", ktrigupdt, ksymbol, kxpos, kypos, kangle, kxspeed, kyspeed, krspeed, kmaccel, kraccel, ksession

ktrigrem, ksymbol, ksession removeObject gituio
printf "remove symbol= %i ksession=%f\n", ktrigrem, ksymbol, ksession

endin

</CsInstruments>
<CsScore>
i 1 0 3600

</CsScore>
</CsoundSynthesizer>















<bsbPanel>
 <label>Widgets</label>
 <objectName/>
 <x>72</x>
 <y>179</y>
 <width>400</width>
 <height>200</height>
 <visible>true</visible>
 <uuid/>
 <bgcolor mode="nobackground">
  <r>231</r>
  <g>46</g>
  <b>255</b>
 </bgcolor>
 <bsbObject version="2" type="BSBVSlider">
  <objectName>slider1</objectName>
  <x>5</x>
  <y>5</y>
  <width>20</width>
  <height>100</height>
  <uuid>{2309e58b-383d-4717-be2c-146e94f5d13d}</uuid>
  <visible>true</visible>
  <midichan>0</midichan>
  <midicc>-3</midicc>
  <minimum>0.00000000</minimum>
  <maximum>1.00000000</maximum>
  <value>0.00000000</value>
  <mode>lin</mode>
  <mouseControl act="jump">continuous</mouseControl>
  <resolution>-1.00000000</resolution>
  <randomizable group="0">false</randomizable>
 </bsbObject>
</bsbPanel>
<bsbPresets>
</bsbPresets>
