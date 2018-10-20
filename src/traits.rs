pub struct Info {
    pub os: String,
    pub arch: String,
    pub bits: usize
}


pub trait Section {
    fn get_flags(&self) -> u32;
    fn get_offset(&self) -> usize;
    fn get_size(&self) -> usize;
}

pub trait Exe<'a> {
    type Item: Section;

    fn get_number_of_sections(&self) -> usize;
    fn get_section_at(&self, idx: usize) -> Option<&Self::Item>;
    fn get_section_name_at(&self, idx: usize) -> Option<&str>;
    fn get_data(&self, start: usize, len: usize) -> &[u8];
    fn get_info(&self) -> Info;
    fn parse(i: &'a [u8]) -> Option<Self> where Self: Sized;
}
