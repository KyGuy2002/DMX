import { Button } from "@/components/ui/button"
import {
  Popover,
  PopoverContent,
  PopoverDescription,
  PopoverHeader,
  PopoverTitle,
  PopoverTrigger,
} from "@/components/ui/popover"
import SettingsDialog from "@/pages/Settings/SettingsDialog"
import { ChevronDown } from "lucide-react"


export default function DeviceDropdown() {

  return (
    <Popover>
        <PopoverTrigger asChild>

            <Button variant="outline" className="w-auto h-10 w-60 px-3">

                <span className="rounded-full h-2 w-2 bg-green-500 mr-2"/>
                
                <div className="text-left mr-auto">
                    <p>Arch Left</p>
                    <p className="text-xs text-muted-foreground -mt-1">
                        PDMX-07af
                    </p>
                </div>

                <SettingsDialog/>

                <Button variant="secondary" size="icon" className="-mr-2 -ml-1">
                    <ChevronDown/>
                </Button>

            </Button>

        </PopoverTrigger>
        <PopoverContent>
            <PopoverHeader>
            <PopoverTitle>Title</PopoverTitle>
            <PopoverDescription>Description text here.</PopoverDescription>
            </PopoverHeader>
        </PopoverContent>
    </Popover>
  )
}