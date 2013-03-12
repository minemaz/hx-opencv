import cv.HighGUI;
import cv.core.*;

class Main {
    static function main() {
        HighGUI.namedWindow("hello1");

        HighGUI.setMouseCallback("hello1", function (event:Int, x:Int, y:Int, flag:Int) {
            trace([event,x,y,flag]);
        });

        var p = new Point(1, 2);
        trace([p.x,p.y]);
        var p = new Point2D32f(1.23456789, 2.3456789101112);
        trace([p.x,p.y]);
        var p = new Point2D64f(1.234567891, 2.3456789101112);
        trace([p.x,p.y]);
        var p = new Point3D32f(1.23456789, 2.3456789101112, 1e-200);
        trace([p.x,p.y,p.z]);
        var p = new Point3D64f(1.234567891, 2.3456789101112, 1e-200);
        trace([p.x,p.y,p.z]);

        HighGUI.waitKey();
    }
}
