#######################################################
## usage: ix1Dorig = zfindX(Hp, W, H)
##
## Finds the corresponding original no transformed
## image points (x,y) from the transformed (x',y') ones

## Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
## Created: 
## Version: 0.0.1
#######################################################
 
function ix1Dorig = zfindX(Hp, W, H)

    %Generate matrix of points in transformed image:
    %   [x1 x2 .. xn]
    %   [y1 y2 .. yn]
    %   [ 1  1 ..  1]
    [xx,yy]=meshgrid([1:W(1)],[1:H(1)]);
    ptransf=[xx(:)';yy(:)';ones(size(xx(:)'))];

    %Find equivalent points in original (no transformed) image by
    %using the inverse of the transformation matrix Hp
    porig = Hp * ptransf;

    %Normalize x1 and x2 by x3. Then make it integer because its an index
    porig = porig ./ porig(3,:);
    porig = int64(porig ./ porig(3,:));

    %Unwrap 2d position matrix into 1d vec [by columns]:  i1d = (i2d.x-1)*H + i2d.y
    ix1Dorig = (porig(1,:) - 1)*H(end) + porig(2,:);

end
