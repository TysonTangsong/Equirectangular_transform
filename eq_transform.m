% parameters : filename, roll, yaw, pitch
% note that r,y,p in radius
% example call: 
% gyro_stabilization('image_DST.tif', 0, pi/4, pi/2);

% function gyro_stabilization(filename, r, y, p)
  filename = '2.jpeg';
  r = pi/4;
  y = 0;
  p = pi/4;
  img_origin=imread(filename);
  img_size = size( img_origin );
  img_undist = zeros( img_size );
  img_undist = uint8( img_undist );
  pnwidth = img_size(2);
  pnheight = img_size(1);
  b = 2*pi;
  mpdistance = pnwidth/b;

  rot = [(mpdistance*pi) (-y*mpdistance)];

  % rotation matrix
  m =[cos(r) sin(r) 0; -cos(p)*sin(r) cos(p)*cos(r) sin(p);sin(r)*sin(p)  -sin(p)*cos(r)  cos(p)];

%  for l0 = 1:img_size(3)
    for l1 = 1:img_size(1)
      x = l1;
      for l2 = 1:img_size(2)
        y =  l2;
        src = [y x];
%         src = src - [pnwidth/2  pnheight/2];%×ø±ê±ä»»
        %src(1) = src(1) + rot(2);
%         src(1)=sign(abs(src(1)-pnwidth/2))*((sign(src(1)-pnwidth/2)+1)*(src(1)-pnwidth/2)-sign(src(1)-pnwidth/2)*src(1))+(1-sign(abs(src(1)-pnwidth/2)))*src(1);
%         src(1)=sign(abs(src(1)+pnwidth/2))*((1-sign(src(1)+pnwidth/2))*(src(1)+pnwidth/2)+sign(src(1)+pnwidth/2)*src(1))+(1-sign(abs(src(1)+pnwidth/2)))*src(1);
%         
        phi = src(1) / mpdistance;
        theta1 = src(2)/ mpdistance - pi/2;

       
        
        v = [cos(phi)*cos(theta1), sin(phi)*cos(theta1),sin(theta1)];
        u=[(v(1)* m(1,1)+v(2)*m(2,1)+v(3)*m(3,1)) (v(1) * m(1,2) + v(2) * m(2,2) + v(3) * m(3,2)) (v(1) * m(1,3) + v(2) * m(2,3) + v(3) * m(3,3))];
        v = u;
        phi = atan2(v(2), v(1));
        theta1 = asin(v(3)) + pi/2;
        src(1) = mpdistance *(atan2(v(2), v(1)) + sign(abs(v(1)))*pi);
        src(2)= mpdistance * (asin(v(3)) + pi/2);
        %resize(2.6666666666666665000, 2.6666666666666665000)
        src(1)  = mod(src(1) + 1500, 3000);
        src = ceil(src+ [0 0]);
        
        if src(2) < 1
          src(2) =1 ;
        end
        if src(2) > pnwidth/2
          src(2) =pnwidth/2 ;
        end
        if src(1) < 1
          src(1) =1 ;
        end
        if src(1) > pnwidth
          src(1) =pnwidth ;
        end
        img_undist(x,y,:) = img_origin(src(2),src(1),:);
        %     if src(2) < 1 || src(2) > 120  || src(1) < 1  src(1) > 240
        %         img_undist(x,y,l0) = img_origin(0,0,l0);
        %     else
        %         img_undist(x,y,l0) = img_origin(src(2),src(1),l0);
        %     end
      end
    end
  
  % compare the two images in one figure
  figure(1);
  subplot(121);
  imshow(img_origin);
  subplot(122);
  imshow(img_undist);

  % save the rotated image
  [pathstr,name,ext] = fileparts(filename)
  imwrite(img_undist, [name, '_rotated', ext]);
  
% end 