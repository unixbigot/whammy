// Fully parametric box for 3D printing
// v4b
// http://www.thingiverse.com/thing:1918169
//
// Original box based on:
// http://tinyurl.com/oznh2p4
//
// Ventilation holes based on:
// http://www.thingiverse.com/thing:1826360

// Configure below
//
/* [Box parameters] */
// Length of box in mm
length = 50;   //[10:254]
// Width of box in mm
width = 32;    //[10:254]
// Height of box in mm
tall = 13;      //[10:254]
// Wall thickness of box in mm
thick = 1.6;      //[1:15]
// Depth of lip on lid in mm
lDepth = 2;     //[1:15]
// Corner radius; 0 = four square corners
cRadius = 5;   //[0:25]   
// Fancy: Square two opposing corners; 0=no, 1=yes (must be using cRadius)
sqCorners = 1;  //[0,1]

/* [Vent Parameters] */
// Vent hole size in mm; 0 = no vents
hSize = 0;      //[0:15]
// Hole Spacing in mm; must be > 0
hSpace = 1;     //[1:15]
// Vent hole type; 0=round, 1=square
hType = 0;      //[0,1]
//
// Configure above

/* [Hidden] */
//fn=200;            // 40 fragments per cicle; comment for draft mode
//$fn=60;
$fs=0.3;
fuz=0.1; fuzz=2*fuz;

// Draw Box
module post() {
/*     difference() {
	  cylinder(d=6, h=6);
	  cylinder(d=2, h=6.5);
     }
*/
     cylinder(d=5,h=1.2);
     cylinder(d=2.4, h=3);
}

module box() {
    translate([cRadius, cRadius, 0]){
	// Create a hollow box
	difference() {
	    // Create box shape
	    roundedBox(length,width,tall,cRadius,sqCorners);
	    // Create negative box shape, smaller by wall thickness plus clearance
	    translate([thick,thick,thick]) { 
		minkowski() { roundedBox(length-(thick*2),width-(thick*2),tall,cRadius,sqCorners);
		    sphere(r=0.4);
		}
		
		// Add vent holes to long sides
		gutterSpace=max(floor(tall*.2),thick*2,cRadius+hSize);
		if (hSize>0) addVentHoles(
		    length,     				// Panel Width
		    tall,						// Panel Height
		    gutterSpace,                // Gutter Width
		    gutterSpace,                // Gutter Height
		    hSize,						// Hole Radius
		    hSpace,					    // Hole Spacing
		    width+0.02,					// Hole Length
		    -(cRadius)-(thick+0.01),	// X Location
		    -gutterSpace,	            // Y Location
		    hType						// Hole Type
		    );
	    }
	    // cut openings in box	
	    //translate([(width-12)/2-cRadius,-cRadius-0.1,thick+2]) cube([12,10,6]);
	    translate([width/2-cRadius,6-cRadius,-fuz]) cylinder(d=10-1,h=5);
	    
	}
    }
    // Add mounting posts inside box
    //translate([(width-9)/2, 10, thick]) post();
    //translate([(width+9)/2, 10, thick]) post();
    translate([4, length-32-thick, 0]) cube([width-12,thick,6]);
}
box();

module lid() {
// Draw Box Lid
// Shift along-side first box
    translate([(width*2)+(10-cRadius), cRadius, 0]) {
	mirror([1,0,0]) {
	    difference() {
		union() {
		    // Create lid
		    roundedBox(length,width,thick,cRadius,sqCorners);
		    // Create lip on top of lid
		    difference() {
			// Create lip shape
			translate([thick,thick,thick]) {
			    roundedBox(length-(thick*2),width-(thick*2),lDepth,cRadius,sqCorners);}
                        // Create negative shape, smaller by wall thickness
                        // (add 0.01 to depth to bypass rendering bug)
                        translate([(thick*2),(thick*2),thick+0.01]) {
                            roundedBox(length-(thick*4),width-(thick*4),lDepth,cRadius,sqCorners);}    
		    }
		}

		// Cut holes in lid
		//translate([width/2-cRadius,length/2-cRadius,thick/2]) cube([15,24,4*thick],center=true);
        	    translate([width/2-cRadius-7,9-cRadius,-fuz]) cylinder(d=4,h=5);

            }
	}
    }
}

//lid();

// Render box according to parameters
module roundedBox(length, width, tall, radius, sqCorners)
{
	if (radius==0) {
		// Draw regular cube
		cube(size=[width,length,tall]);
	} else {
		// Draw rounded corner cube
		diam=2*radius;

		// Draw base rounded shape
		minkowski() {
			cube(size=[width-diam,length-diam, tall/2]);
			cylinder(r=radius, h=tall/2);
		}

        // Square opposing corners
		if (sqCorners==1) {
			// Corner cube bottom right
			translate([width-diam,-radius,0]) {cube(size=[radius,radius,tall]);}

			// Corner cube top left
			translate([-radius,length-diam,0]) {cube(size=[radius,radius,tall]);}  
		}
	}
}

// Ventillate opposing panels Y-axis panels
module addVentHoles (pWidth, pHeight, gWidth, gHeight, hSize, hSpace, hLength, xLoc, yLoc, hType) {
	hSpacing = hSpace + hSize;
    mainRowHoles = floor((pWidth - (2 * gWidth)) / hSpacing);
    rows = floor((pHeight - (2 * gHeight)) / hSpacing);

    for (n = [0:rows]) {
        for (i = [0:mainRowHoles]) {
            // If we're on an even-number row, then we put holes across the panel
            if (n % 2 == 0)
                translate([
					xLoc,
					yLoc+(i*hSpacing)+(gWidth),
					(n*hSpacing)+gHeight])
					
					rotate([0,90,0])
                    // Choose square or round hole
					if (hType==0) {
						cylinder(h=hLength, r=hSize/2);
					} else {
						translate([0,0,hLength/2]) cube(size=[hSize,hSize,hLength], center=true);
					}	

            // If we're on an odd-number row then we offset the holes, and drop one
            if ((n % 2 == 1) && (i<mainRowHoles)) // This drops the hole
				translate([
					xLoc,
					yLoc+(i*hSpacing)+(gWidth)+(hSpacing/2),
					(n*hSpacing)+gHeight]) 
					
					rotate([0,90,0])
					// Choose square or round hole
					if (hType==0) {				 
						cylinder(h=hLength, r=hSize/2);
					} else {
						translate([0,0,hLength/2]) cube(size=[hSize,hSize,hLength], center=true);
					}	
        }
    }
}
