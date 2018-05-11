#[macro_export]
macro_rules! generate_c_api {
    ($st:ty, $et:ty) => (
        use exe::{Section, Exe};

        #[no_mangle]
        pub extern fn rs_get_flags<'a>(section_h: *mut ::libc::c_void) -> ::libc::uint32_t {
            assert_ne!(section_h, ::std::ptr::null_mut());
            let section = unsafe { Box::from_raw(section_h as *mut $st) };
            let ret = section.get_flags() as ::libc::uint32_t;
            Box::into_raw(section);
            ret
        }

        #[no_mangle]
        pub extern fn rs_get_offset<'a>(section_h: *mut ::libc::c_void) -> ::libc::size_t {
            assert_ne!(section_h, ::std::ptr::null_mut());
            let section = unsafe { Box::from_raw(section_h as *mut $st) };
            let ret = section.get_offset() as ::libc::size_t;
            Box::into_raw(section);
            ret
        }

        #[no_mangle]
        pub extern fn rs_get_size<'a>(section_h: *mut ::libc::c_void) -> ::libc::size_t {
            assert_ne!(section_h, ::std::ptr::null_mut());
            let section = unsafe { Box::from_raw(section_h as *mut $st) };
            let ret = section.get_size() as ::libc::size_t;
            Box::into_raw(section);
            ret
        }

        #[no_mangle]
        pub extern fn rs_get_number_of_sections<'a>(exe_h: *mut ::libc::c_void) -> ::libc::size_t {
            assert_ne!(exe_h, ::std::ptr::null_mut());
            let e = unsafe { Box::from_raw(exe_h as *mut $et) };
            let ret = e.get_number_of_sections();
            Box::into_raw(e);
            ret
        }

        #[no_mangle]
        pub extern fn rs_get_section_at<'a>(exe_h: *mut ::libc::c_void, idx: usize) -> *const $st {
            assert_ne!(exe_h, ::std::ptr::null_mut());
            let e = unsafe { Box::from_raw(exe_h as *mut $et) };
            let ret = match e.get_section_at(idx) {
                Some(s) => s as *const $st,
                None => ::std::ptr::null()
            };
            Box::into_raw(e);
            ret
        }

        #[no_mangle]
        pub extern fn rs_get_section_name_at<'a>(exe_h: *mut ::libc::c_void, idx: usize) -> *const ::libc::c_char {
            assert_ne!(exe_h, ::std::ptr::null_mut());
            let e = unsafe { Box::from_raw(exe_h as *mut $et) };
            let ret = match e.get_section_at(idx) {
                Some(s) => s.name.as_ptr() as *const ::libc::c_char,
                None => ::std::ptr::null()
            };
            Box::into_raw(e);
            ret
        }

        #[no_mangle]
        pub extern fn rs_get_data<'a>(exe_h: *mut ::libc::c_void, start: usize, len: usize) -> *const ::libc::uint8_t {
            assert_ne!(exe_h, ::std::ptr::null_mut());
            let e = unsafe { Box::from_raw(exe_h as *mut $et) };
            let ret = e.get_data(start, len).as_ptr() as *const ::libc::uint8_t;
            Box::into_raw(e);
            ret
        }

        #[no_mangle]
        pub extern fn rs_free_exe<'a>(exe_h: *mut ::libc::c_void) {
            assert_ne!(exe_h, ::std::ptr::null_mut());
            let _e = unsafe { Box::from_raw(exe_h as *mut $et) };
            //::std::mem::drop(e);
        }
    )
}


