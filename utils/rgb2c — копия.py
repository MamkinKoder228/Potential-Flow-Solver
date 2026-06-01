from os import listdir

out = open("colormaps.c", "w")
out.write("#define ValToColor(id, cmap) COLOR(cmap ## Red[(size_t)(255 * id)], cmap ## Green[(size_t)(255 * id)], cmap ## Blue[(size_t)(255 * id)])\n\n")

colormaps = listdir("colormaps")

for filename in colormaps:
	if filename[-3:] != "rgb":
		continue

	cmap = open("colormaps/%s" % filename, "r").read().split("\n")
	name = filename[0:-4]
	ncolors = int(cmap[0][8:])

	R = []
	G = []
	B = []

	FloatRGB = False
	if "".join(cmap).count(".") + "".join(cmap).count(",") > 0:
		FloatRGB = True

	for i in range(ncolors):
		if FloatRGB:
			r, g, b = [str(int(c * 255)) for c in map(float, cmap[i + 2].split())]
		else:
			r, g, b = cmap[i + 2].split()
		R.append(r)
		G.append(g)
		B.append(b)

	out.write("// %s colormap RGB table\n" % name)
	out.write("#define %sCOLORS %d\n" % (name, ncolors))
	out.write("unsigned char %sRed[%d] = {%s};\n" % (name, ncolors, ", ".join(R)))
	out.write("unsigned char %sGreen[%d] = {%s};\n" % (name, ncolors, ", ".join(G)))
	out.write("unsigned char %sBlue[%d] = {%s};\n\n" % (name, ncolors, ", ".join(B)))