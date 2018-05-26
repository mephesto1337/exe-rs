#[repr(C)]
pub struct CSection {
    pub name:    *const ::libc::c_char,
    pub flags:   ::libc::uint32_t,
    pub paddr:   ::libc::size_t,
    pub vaddr:   ::libc::size_t,
    pub size:    ::libc::size_t,
}

#[macro_export]
macro_rules! generate_c_api {
    ($et:ty, $rs_get_number_of_sections:ident, $rs_get_section_at:ident, $rs_get_data:ident, $rs_free_section:ident, $rs_free_exe:ident) => (
        use exe::{Section, Exe};

        #[no_mangle]
        pub extern fn $rs_get_number_of_sections<'a>(exe_h: *mut ::libc::c_void) -> ::libc::size_t {
            assert_ne!(exe_h, ::std::ptr::null_mut());
            let e = unsafe { Box::from_raw(exe_h as *mut $et) };
            let ret = e.get_number_of_sections();
            Box::into_raw(e);
            ret
        }

        #[no_mangle]
        pub extern fn $rs_get_section_at<'a>(exe_h: *mut ::libc::c_void, idx: usize) -> *const ::libc::c_void {
            assert_ne!(exe_h, ::std::ptr::null_mut());
            let e = unsafe { Box::from_raw(exe_h as *mut $et) };
            let ret = match e.get_section_at(idx) {
                Some(s) => Box::into_raw(Box::new($crate::CSection {
                    name:   match e.get_section_name_at(idx) {
                        Some(n) => n.as_ptr() as *const ::libc::c_char,
                        None => ::std::ptr::null() as *const ::libc::c_char
                    },
                    flags:  s.get_flags() as ::libc::uint32_t,
                    paddr:  s.get_offset() as ::libc::size_t,
                    vaddr:  s.get_offset() as ::libc::size_t,
                    size:   s.get_size() as ::libc::size_t
                })) as *const ::libc::c_void,
                None => ::std::ptr::null()
            };
            Box::into_raw(e);
            ret
        }
        
        #[no_mangle]
        pub extern fn $rs_get_data<'a>(exe_h: *mut ::libc::c_void, start: usize, len: usize) -> *const ::libc::uint8_t {
            assert_ne!(exe_h, ::std::ptr::null_mut());
            let e = unsafe { Box::from_raw(exe_h as *mut $et) };
            let ret = e.get_data(start, len).as_ptr() as *const ::libc::uint8_t;
            Box::into_raw(e);
            ret
        }

        #[no_mangle]
        pub extern fn $rs_free_section<'a>(section_h: *mut ::libc::c_void) {
            assert_ne!(section_h, ::std::ptr::null_mut());
            let _s = unsafe { Box::from_raw(section_h as *mut $crate::CSection) };
        }

        #[no_mangle]
        pub extern fn $rs_free_exe<'a>(exe_h: *mut ::libc::c_void) {
            assert_ne!(exe_h, ::std::ptr::null_mut());
            let _e = unsafe { Box::from_raw(exe_h as *mut $et) };
            //::std::mem::drop(e);
        }
    )
}


