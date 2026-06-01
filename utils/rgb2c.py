from os import listdir

out = open("../colormaps.c", "w")
out.write("#define ValToColor(id, cmap) COLOR(ValueToRedColor[cmap][(size_t)(ColormapColorCount[cmap] * id)], ValueToGreenColor[cmap][(size_t)(ColormapColorCount[cmap] * id)], ValueToBlueColor[cmap][(size_t)(ColormapColorCount[cmap] * id)])\n\n")

colormaps = listdir("../colormaps")
R = []
G = []
B = []
colorcount = []
names = []

for filename in colormaps:
	if filename[-3:] != "rgb":
		continue

	cmap = open("../colormaps/%s" % filename, "r").read().split("\n")
	name = filename[0:-4]
	ncolors = int(cmap[0][8:])

	colorcount.append(ncolors)
	names.append("CMAP_%s" % name.upper())

	R.append([])
	G.append([])
	B.append([])

	FloatRGB = False
	if "".join(cmap).count(".") + "".join(cmap).count(",") > 0:
		FloatRGB = True

	for i in range(ncolors):
		if FloatRGB:
			r, g, b = [str(int(c * 255)) for c in map(float, cmap[i + 2].split())]
		else:
			r, g, b = cmap[i + 2].split()
		R[-1].append(r)
		G[-1].append(g)
		B[-1].append(b)

	# out.write("#define CMAP_%s %d\n" % (name, len(colorcount) - 1))
out.write("enum {\n\t%s\n};\n" % ",\n\t".join(names))
out.write("\nunsigned char ValueToRedColor[%d][%d] = {%s};\n" % (len(R), max(colorcount), ", ".join(["{%s}" % ", ".join(r) for r in R])))
out.write("unsigned char ValueToGreenColor[%d][%d] = {%s};\n" % (len(G), max(colorcount), ", ".join(["{%s}" % ", ".join(r) for r in G])))
out.write("unsigned char ValueToBlueColor[%d][%d] = {%s};\n" % (len(B), max(colorcount), ", ".join(["{%s}" % ", ".join(r) for r in B])))
out.write("size_t ColormapColorCount[%d] = {%s};\n" % (len(colorcount), ", ".join(map(str, colorcount))))