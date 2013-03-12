deleteall;

addrect;
set("x",0);
set("x span",lx);
set("y span",base_width);
set("y",radius+gap+base_width);
#copy(0,-2*(radius+gap+base_width),0);

copy;
set("y",radius);
set("x span",Lc);

copy;
set("y",-radius);

addring;
set("theta start",-90);
set("theta stop",90);
set("inner radius",radius-base_width/2);
set("outer radius",radius+base_width/2);
set("x",Lc/2);

copy;
set("theta start",90);
set("theta stop",270);
set("x",-Lc/2);


selectall;
set("material",material);
if(material=="<Object defined dielectric>") {
  set("index",index);
}
set("z min",0);
set("z max",height);
