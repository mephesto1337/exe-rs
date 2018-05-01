pub trait Section {
    fn get_flags(&self) -> u32;
    fn get_offset(&self) -> usize;
    fn get_size(&self) -> usize;
}

pub trait Exe {
    type Item: Section;

    fn get_number_of_sections(&self) -> usize;
    fn get_section_at(&self, idx: usize) -> Option<&Self::Item>;
}
