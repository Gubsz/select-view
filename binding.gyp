{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ 
      	"addon.cc" 
      ],

	  "libraries": [
		'-lopencv_core', 
		'-lopencv_imgproc', 
		'-lopencv_calib3d',
		'-lopencv_features2d', 
		'-lopencv_objdetect', 
		'-lopencv_contrib', 
		'-lopencv_flann', 
		'-lopencv_ml', 
		'-lopencv_legacy'
	  ]
    }
  ],
}
