require 'nokogiri'
require 'pry-byebug'

arduino_array = []

# Opens kml file, removes namespaces, extracts placemarks which is the area
	# that holds the POI info, iterates through to create proper arduino data structure
	# of nested arrays

doc = File.open("POIs.kml") { |f| Nokogiri::XML(f)}
doc.remove_namespaces!
placemarks = doc.xpath("//Folder/Placemark")
placemarks.each do | placemark |
	name = placemark.search("name").text
	lat = placemark.search('coordinates').text.split("\n")[1].split(',').first.lstrip!
	lon = placemarks[0].search('coordinates').text.split("\n")[1].split(',')[1]
	arr = [name, lat, lon]
	arduino_array.push(arr)
end

number_of_points = arduino_array.length

output = File.open("points.txt", "w") do | line | 
	line.puts "Poi poi_list[" + arduino_array.length.to_s + "] = {"
	arduino_array.each do | point | 
		line.puts "{" + "\""  + point[0].rstrip + "\"" + "," +  point[1].strip + "," + point[2].strip + "}"
		if arduino_array[-1] != point
			line.puts ","
		end
	end
	line.puts "};"
	number_of_points = arduino_array.length
	line.puts "int number_of_points = " + number_of_points.to_s + ";"
end

# const double EIFFEL_TOWER_LAT = 36.96070;
# const double EIFFEL_TOWER_LNG = 127.05692;

# open the file. iterate through the points
# for each point, copy latitude into a variable, copy longitude into variable
# copy name into a variable. push those into a hash and that hash into an 
#array
# so [{name: px main, latitude: 123, longitude:345},{},]

#after i have the data structure, then iterate through it and create the necessary
#variables

# array string eifel[Eiffel_Tower_LAT, EIFFEL_TOWER_LNG, Eiffel_NAME]
# Example - 

# int pinMatrix[3][3] = {
 
#   {2,  3,  4  },
 
#   {5,  6,  7  },
 
#   {8,  9, 10  }
 
# };

# so, iterate through, get the name, lat, long, store them into an array and push 
# it 