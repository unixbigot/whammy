/* [Size] */

// length of circuit board
length = 21;

// width of circuit board
width = 32;

// height of circuit board
height=4+2.6;

// height of battery holder
battheight = 2.6;

// corner radius
radius=1;

// fit allowance between base and lid
fit=0.2;

/* [Wall thicknesses] */
side=1.2;
bottom=1.2;
lidbottom=5;
top=0;
rim = 2;

/* [Options] */

// place magnets on outside of case
magnets = "none"; // [none:No magnets,inside:magnets inside case,outside:magnets outside case]

// hole for LED
led_hole = true;

// hole for suction cup
keyhole = true;

// include holes for programming cable
pogo=false;

// length of magnet
maglength = 20;

// height of magnet
magheight = 6;

// width of magnet
magwidth = 4.4;

// fit allowance for magnet socket
magfit = 0.4;

// suction cup keyhole size
keyhole_large = 8;
keyhole_small = 5;
keyhole_depth = 2;



/* [Hidden] */
led_inset_x=4;
led_inset_y = 7;

lidheight=height-bottom+lidbottom;

fuz=0.1; 
fuzz=2*fuz;
$fn=60;

module lozenge(l,w,h,r) {
    hull() {
        translate([r,r,0]) cylinder(r=r,h=h);
        translate([l-r,r,0]) cylinder(r=r,h=h);
        translate([l-r,w-r,0]) cylinder(r=r,h=h);
        translate([r,w-r,0]) cylinder(r=r,h=h);
    }
}

module hollow_lozenge(l,w,h,r=radius,s=side,b=bottom,t=top) {
    difference() {
	lozenge(l,w,h,r);
	translate([s,s,b]) lozenge(l-2*s,w-2*s,h-b-t,r);
    }
}

module hollow_lozenge_id(l,w,h,r,s=side,b=bottom,t=top) {
    difference() {
	lozenge(l+2*s,w+2*s,h+b+t,r+s);
	translate([s,s,b]) lozenge(l,w,h+fuzz,r);
    }
}

module base(){
// inner shell
    difference() {
        union() {
            if (magnets=="inside") {
		// magnets at end
                hollow_lozenge_id(length, width+2*magwidth+2*side, height, radius);
                translate([side+radius,side,bottom]) cube([length-2*radius,magwidth,height-battheight-bottom]);
                translate([side+radius,side+magwidth+side+width+side,bottom]) cube([length-2*radius,magwidth,height-battheight-bottom]);
                translate([0,side+magwidth,0]) cube([length+2*side,side,bottom+height]);
                translate([0,width+magwidth+2*side,0]) cube([length+2*side,side,bottom+height]);
                translate([-side-fit,-side-fit,0]) hollow_lozenge(
                    length+2*(side+fit+side), 
                    width+2*(magwidth+side+side+fit+side), 
                    rim, 
                    radius+(side+fit+side),
                    side+fit,
                    bottom
                );

            } else if (magnets=="outside") {
		// magnets at sides, outside case
                hollow_lozenge_id(length, width+2*magwidth+2*side, height, radius);
                translate([-side-fit,-side-fit,0]) hollow_lozenge(length+2*(side+fit+side), width+2*(side+fit+side), bottom, radius+(side+fit+side),2*side,bottom);
            }
	    else {
		// no magnets
                hollow_lozenge_id(length, width, battheight, radius);
	    }

	    // base to provide seal against lid
            translate([-side-fit,-side-fit,0]) hollow_lozenge(length+2*(side+fit+side), width+2*(side+fit+side), bottom, radius+(side+fit+side),2*side,bottom);
        }
        if (led_hole) {
            translate([side+length-led_inset_x,side+magwidth+side+width-led_inset_y,bottom/2]) cube([1.6,1.6,2*bottom+2*fuzz],center=true);
        }
	if (magnets == "inside") {
	    // cut out to allow lifting the PCB
	    translate([-fuzz,side+magwidth+side,bottom+battheight]) cube([length+2*side+2*fuzz,width,height-battheight+fuzz]);
	}
        
        if (pogo) {
            translate([side+length-4,side+1,-fuzz]) {
                for (i=[0:5]) {
                    translate([-2*i,0,0]) cylinder(d=1,h=2*bottom+2*fuzz);
                }
            }
        }
        
    }
}

module vertical_text(t, size) {
     rotate([90,180,0]) {
          linear_extrude(height=0.8) {
               text(text=t,
                    size = size,
                    //font = "Verdana:style=Bold"
                    font = "Avenir Next Condensed:style=Demi Bold"
                 );
          }    
     }
}

module magbox() {
    difference() {
	cube([maglength+magfit+2*side, magwidth+magfit+2*side, magheight+bottom]);
	translate([side,side,-fuzz]) cube([maglength+magfit,magwidth+magfit,magheight+fuzz]);
    }
}

module lid() {
    difference() {
        union() {
    // outer shell
        if (magnets=="outside") {
            hollow_lozenge_id(l=length+2*(side+fit),w=width+2*(side+fit),h=lidheight,r=radius+(side+fit),b=lidbottom);
        } else if (magnets=="inside") {
            hollow_lozenge_id(l=length+2*(side+fit),w=width+2*(side+magwidth)+2*(side+fit),h=lidheight,r=radius+(side+fit),b=lidbottom);
        } else {
	    ll = length+2*(side+fit);
	    lw = width+2*(side+fit);
	    echo (ll=ll,lw=lw,height=height,side=side,bottom=bottom,lidbottom=lidbottom,lidheight=lidheight);
	    	
            hollow_lozenge_id(l=ll,w=lw,h=height,r=radius+(side+fit),b=lidbottom);
        }
       
        if (magnets=="outside") {
            overlap = 1;
            translate([side,lidwidth_outer/2-12,0]) rotate([0,0,90]) magbox();
            translate([lidlength_outer-overlap,lidwidth_outer/2-12,0]) rotate([0,0,90]) scale([1,-1,1]) magbox();
        }
        //translate([-2,lidwidth_outer/2-12,0]) cube([2+overlap,2,lidheight]);
        //translate([-2,lidwidth_outer/2+10,0]) cube([2+overlap,2,lidheight]);
        //translate([lidlength_outer-overlap,lidwidth_outer/2-12,0]) cube([2+overlap,2,lidheight]);
        //translate([lidlength_outer-overlap,lidwidth_outer/2+10,0]) cube([2+overlap,2,lidheight]);

        //translate([width-3,0,7]) vertical_text(" f742d1",5);
    }
    if (keyhole) {
           translate([2*side+length/2,side+width/2,-fuz]) {
                cylinder(d=keyhole_large, h=lidbottom+fuzz);
		hull() {
		    cylinder(d=keyhole_small, h=lidbottom+fuzz);
		    translate([0,-keyhole_large/2-keyhole_small/2,0]) cylinder(d=keyhole_small, h=lidbottom+fuzz);
		}
		translate([0,0,3]) 
		hull() {
		    cylinder(d=keyhole_large, h=lidbottom-3+fuzz);
		    translate([0,-keyhole_large/2-keyhole_small/2,0]) cylinder(d=keyhole_large, h=lidbottom-3+fuzz);
		}
            }
        }

    }
}


rotate([0,0,90]) base();
translate([2*length+5,-side,0]) rotate([0,0,90]) lid();
