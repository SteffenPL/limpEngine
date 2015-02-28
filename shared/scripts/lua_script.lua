

dim = 20
box = Box.new(dim*dim+1)
n = dim-1

for i = 0,n do
    for j = 0,n do
        box:setPos(i+dim*j,2.5*(i-n/2),0,2.5*(j-n/2))

    end
end


function f(x,y)

  x = math.cos(phi)*math.sin(theta)
  y = math.sin(phi)*math.sin(theta)
  z = math.cos(theta)

    return x,y,z
end

function g(x,y)

  x = math.modf(x,10)
  y = math.modf(y,10)

  z = x*y

    return x,y,z
end

function update_butterfly()
    boxPerButterfly = 10;

    n = box:getBoxCount() -1
    for i = 0,n do
        x = box:getX(i)
        y = box:getY(i)
        z = box:getZ(i)


        size = 20 + 10*math.sin( time )

        j , k = math.mod(i,boxPerButterfly) , math.floor( i/boxPerButterfly )


        phi = math.sin(time*time + j  )
        theta = math.cos(time - j )

        x,y,z = f(phi,theta)

        x,y,z = x*size,y*size,z*size

        x = x + size*k
        y = y + size*k
        z = z + size*k



        box:setPos(i,x,y,z)
    end
    box:setPos(0,char_x,math.sin(char_x+char_z),char_z)

    return 0
end

function update()

    n = box:getBoxCount() -1
    for i = 0,n-1 do


        x = box:getX(i)
        y = box:getY(i)
        z = box:getZ(i)




        dx = char_x - x
        dz = char_z - z

        len = dx*dx + dz*dz
        if( dx*dx + dz*dz < 6 ) then
            --len = math.sqrt(len)
            --dx = dx / len
            --dz = dz / len
            --x = x - dx
            --z = z - dz
            y = 8
        else
          if y > 0 then y = y - 0.1 end
          end

        box:setPos(i,x,y,z)
    end
    box:setPos(n,char_x,0,char_z)

    return 0
end

