#include <stdio.h>
#include <math.h>
#include "dislin.h"
#include "discpp.h"
#include "interface.h"
#include <cstring>
#include <string>

d_interface::d_interface(){
}

void d_interface::widget (void)
{ int ip, ip_l, ip_r, idev = 1, iopt = 1;
  std::string chelp;
  
  chelp = "Q-learning algorithm with safety|";
  chelp += "Author: Anton Naruta|";
  chelp += "TU Delft";

  cl1 += "Test CMAC learning";

  g.swgtit ("RL Algorithm");
  g.swghlp (chelp.c_str ());
  g.swgwth (30);
  g.swgtyp ("SCROLL", "LIST");
  g.swgpop ("NOOK");
  
  ip = g.wgini ("hori");
  ip_l = g.wgbas (ip, "vert");
  ip_r = g.wgbas (ip, "vert");
  g.wglab(ip_l, "Run test:");
  id_lis1 = g.wglis(ip_l,cl1.c_str(),iopt);
  id_pbut = g.wgpbut(ip_l, "Go!");

  g.swgdrw (1.0f);
  int id_draw = g.wgdraw(ip_r);

  g.setxid(id_draw, "WIDGET");
  //g.metafl("xwin");
  //g.wglab (ip_l, "Example:");
  //id_lis1 = g.wglis (ip_l, cl2.c_str (), iopt);
  
  //g.wglab (ip_r, "Device:");
  //id_lis2 = g.wglis (ip_r, cl1.c_str (), idev);
  
  //id_pbut = g.wgpbut (ip_r, "Plot");
  ////g.swgcbk (id_pbut, mysub);

  g.wgquit (ip_l);
  g.wgfin ();
}  
