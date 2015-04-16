load "$NCARG_ROOT/lib/ncarg/nclscripts/csm/gsn_code.ncl"
load "$NCARG_ROOT/lib/ncarg/nclscripts/csm/gsn_csm.ncl"
load "$NCARG_ROOT/lib/ncarg/nclscripts/csm/contributed.ncl"

begin

	file_path = "uff.nc"

	f = addfile(file_path, "r")

	wks = gsn_open_wks("pdf", "uff")

	gsn_define_colormap(wks, "MPL_Greys")

	res = True
	res@gsnAddCyclic = False
	res@cnLinesOn = False
	res@cnFillOn = True
	res@cnFillMode = "RasterFill"
	res@gsnSpreadColors = True
	res@cnLabelBarEndStyle = "ExcludeOuterBoxes"
	res@mpMinLonF = min(f->lon)+3.5
	res@mpMaxLonF = max(f->lon)-3.9
	res@mpMinLatF = min(f->lat)+4.1
	res@mpMaxLatF = max(f->lat)-2.85
	res@pmTickMarkDisplayMode = "Always"

	plot = gsn_csm_contour_map_ce(wks, f->h0, res)

end
