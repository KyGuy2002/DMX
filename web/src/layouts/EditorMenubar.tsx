import {
  Menubar,
  MenubarContent,
  MenubarGroup,
  MenubarItem,
  MenubarMenu,
  MenubarSeparator,
  MenubarShortcut,
  MenubarTrigger,
} from "@/components/ui/menubar"

import logoImg from "@assets/logo.png"


export default function EditorMenubar() {

  return (
    <Menubar>

        <MenubarMenu>
            <MenubarTrigger>File</MenubarTrigger>
            <MenubarContent>
            <MenubarGroup>
                <MenubarItem>
                New Tab <MenubarShortcut>⌘T</MenubarShortcut>
                </MenubarItem>
                <MenubarItem>New Window</MenubarItem>
            </MenubarGroup>
            <MenubarSeparator />
            <MenubarGroup>
                <MenubarItem>Share</MenubarItem>
                <MenubarItem>Print</MenubarItem>
            </MenubarGroup>
            </MenubarContent>
        </MenubarMenu>

        <MenubarMenu>
            <MenubarTrigger>Edit</MenubarTrigger>
        </MenubarMenu>

        <MenubarMenu>
            <MenubarTrigger>View</MenubarTrigger>
        </MenubarMenu>

        <MenubarMenu>
            <MenubarTrigger>Pattern</MenubarTrigger>
        </MenubarMenu>

        <MenubarMenu>
            <MenubarTrigger>Help</MenubarTrigger>
        </MenubarMenu>
    </Menubar>
  )
}