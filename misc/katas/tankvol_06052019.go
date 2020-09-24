/*

*/
package kata
import "math"

func TankVol(h, d, vt int) int {
   
    var df float64 = float64(d);
    var hf float64 = float64(h);
    var vtf float64 = float64(vt);
    
    var r float64= float64(df/2);
    
    // take the slice and subtract the triangle
    var circleArea float64 = math.Pi*(r*r);
    var lngth float64 = vtf/circleArea;
    
    var theta float64;
    var liquidArea float64 = 0.0;
    var triangleArea float64;
    var sliceArea float64;
    var emptyArea float64;
    var leg float64 = 0.0;
    
    if hf<r {
      theta = 2*math.Acos((r-hf)/r);
      // aaaaaaaah we need the other leg
      leg = math.Sqrt((r*r) - ((r-hf)*(r-hf)))
      triangleArea = ((r-hf) * leg);
      sliceArea = (theta / (math.Pi)) * (circleArea/2);
      liquidArea = (sliceArea - triangleArea);
    } else{
      theta = 2*math.Acos((hf-r)/r);
      leg = math.Sqrt((r*r) - ((hf-r)*(hf-r)))
      triangleArea = ((hf-r) * leg);
      sliceArea = (theta / (math.Pi)) * (circleArea/2);
      emptyArea = sliceArea - triangleArea;
      liquidArea = (circleArea/2) + ((circleArea/2) - emptyArea);

    }
    
    var liquidVolume float64 = liquidArea * lngth;
    liquidVolume = math.Floor(liquidVolume);
    
    var retval = int(liquidVolume);
    print("\nTotal volume: ",  vt);
    print("\nLiquid height: ",  h);
    print("\nTank diameter: ", d);
    print("\nLength: ",  lngth);
    
    print("\nCircle area: ",  circleArea);
    print("\nTheta: ",  theta);
    print("\nSlice area: ",  sliceArea);
    print("\nTriangle area: ",  triangleArea);
    print("\nLiquid area: ",  liquidArea);
    
    print("\nLiquid volume: ", liquidVolume);
    
    return retval;
}