length=25.4;
width=20.5;
clearance=0.2;
$fn=60;
fuz=0.1;fuzz=2*fuz;
include <lozenge.scad>

module post() {
    cylinder(d=5,h=8);
    cylinder(d=4,h=width+fuzz);
    translate([0,0,15]) cylinder(d=8,h=10);
}


module base() {
    difference() {
	lozenge(l=length*2,w=width*2,h=width,r=5);
	translate([5,5,-fuz]) post();
	translate([length*2-5,5,-fuz]) post();
	translate([5,2*width-5,-fuz]) post();
	translate([length*2-5,2*width-5,-fuz]) post();
	translate([length/2,width/2,5]) lozenge(l=length+2*clearance,w=width+2*clearance,h=width,r=2);
	translate([-fuz,width/2,5]) {
	    //cube([length*1.5+fuz,width,1]);
	    translate([0,1.5,-2.5]) cube([length/2+fuzz, width-3,5]);
	}
	translate([length,width,width]) rotate([0,90,0]) cylinder(d=width*0.75,h=3*length,center=true);
    }
}

//base();
intersection() {
    base();
    translate([-fuz,-fuz,-fuz]) cube([3*length,3*width,7.5]);
}

translate([3*length,0,0]) intersection() {
    translate([0,0,-7.5-fuz]) base();
    translate([-fuz,-fuz,0]) cube([3*length,3*width,width]);
}
