#######################################################
## usage: ix1Dorig = zfindX(Hinv, W, H)
##
## Finds the corresponding original no transformed
## image points (x,y) from the transformed (x',y') ones

## Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
## Created: 
## Version: 0.0.1
#######################################################
 
function ix1Dorig = zfindX(Hinv, W, H)

    %Generate matrix of points in transformed image:
    %   [x1 x2 .. xn]
    %   [y1 y2 .. yn]
    %   [ 1  1 ..  1]
    [xx,yy]=meshgrid([1:W],[1:H]);
    ptransf=[xx(:)';yy(:)';ones(size(xx(:)'))];

    %Find equivalent points in original (no transformed) image by
    %using the inverse of the transformation matrix Hinv
    porig = Hinv * ptransf;

    %Normalize x1 and x2 by x3. Then make it integer because its an index
    porig = int32(porig ./ porig(3,:));

    %Find index in 1d vec of the 2d points of image:  i1d = (i2d.x-1)*W + i2d.y
    ix1Dorig = (porig(1,:) - 1)*H + porig(2,:);

end
