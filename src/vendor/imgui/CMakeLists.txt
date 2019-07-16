add_library(imgui STATIC 
	imgui.cpp		imgui.h
	imgui_impl_glfw.cpp	imgui_impl_glfw.h

	imgui_draw.cpp  
	imgui_demo.cpp  
	imgui_widgets.cpp  
	
	imgui_internal.h
	imconfig.h      

	imstb_rectpack.h  
	imstb_truetype.h
	imstb_textedit.h

	imgui_impl_opengl3.cpp	imgui_impl_opengl3.h    
)
